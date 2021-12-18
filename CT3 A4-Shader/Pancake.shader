Shader "Custom/swirl"
{
    Properties
    {
        _Color ("Color", Color) = (1,1,1,1)
        _MainTex ("Albedo (RGB)", 2D) = "white" {}
        _Glossiness ("Smoothness", Range(0,1)) = 0.5
        _Metallic ("Metallic", Range(0,1)) = 0.0
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 200

        CGPROGRAM
        // Physically based Standard lighting model, and enable shadows on all light types
        #pragma surface surf Standard fullforwardshadows vertex:vert

        // Use shader model 3.0 target, to get nicer looking lighting
        #pragma target 3.0

        sampler2D _MainTex;

        struct Input
        {
            float2 uv_MainTex;
        };

        half _Glossiness;
        half _Metallic;
        fixed4 _Color;

        // Add instancing support for this shader. You need to check 'Enable Instancing' on materials that use the shader.
        // See https://docs.unity3d.com/Manual/GPUInstancing.html for more information about instancing.
        // #pragma instancing_options assumeuniformscaling
        UNITY_INSTANCING_BUFFER_START(Props)
            // put more per-instance properties here
        UNITY_INSTANCING_BUFFER_END(Props)
                // http://www.iquilezles.org/www/articles/palettes/palettes.htm 
        // to see this function graphed out go to: https://www.desmos.com/calculator/rz7abjujdj
        float3 cosPalette( float t , float3 brightness, float3 contrast, float3 osc, float3 phase)
        {

            return brightness + contrast*cos( 6.28318*(osc*t+phase) );
        }

        // smooth mod(x,y) with radius of e
        float smoothMod(float x, float y, float e){
            float PI = 3.1415;
            float top = cos(PI * (x/y)) * sin(PI * (x/y));
            float bot = pow(sin(PI * (x/y)),2.)+ pow(e, sin(_Time.y));
            float at = atan(top/bot);
            return y * (1./2.) - (1./PI) * at ;
        }
        float3 swirl(float2 pos){
            pos.y = smoothMod(pos.y,0.5,0.1);
            pos.x = smoothMod(pos.x,0.5,0.1);
            //float2 pos = (_ScreenParams.xy - float2(0.5,0.5))*float2(2.0,2.0);
            float angle = atan2(pos.x, pos.y);
            // we take the absolute value of x and add a small number to avoid
            // dividing by zero which is undefined behavior
            
            float r = (sin(_Time.y + angle));
            // sin returns a number from -1 to 1, and colors are from 0 to 1, so thats 
            // why you only see red on the screen half the time. the angle goes around
            // the screen, adding time moves it clockwise
            
            float ringFrequency = 5.; // making this number bigger will increase frequency
            float g = cos(length(pos*ringFrequency) ); 
            // the distance (aka length) from the center put in a cos, time moves the 
            // circles in. 
            
            float b = cos(angle+ cos(length(pos*90.)) ); 

                        // this combines what we learned in the red and green channels
            // angle is going through a cos and so is the length, so we see the 
            // blue channel oscillating in both dimensions the polar coordinates give us
            float3 brightness = float3(1,0,0);
            float3 contrast = float3(7.0, 7.0, 7.0);
            // the numbers that divide time are pretty arbitrary, as long as they are not the same and are 
            // somewhere between 10-100 id say it gives the desired effect
            float3 osc = float3(r,r,r);
            float3 phase = float3(g, cos(_Time.y/23.), sin(_Time.y/3.));
            float3 color = cosPalette(g, brightness, contrast, osc, phase);

            return color = float4(color.r, color.g, color.b,1.);

            return color;
        }
        
        void vert (inout appdata_full v) {
           // Do whatever you want with the "vertex" property of v here
           float3 worldPos = mul (unity_ObjectToWorld, v.vertex);
           fixed3 swl = swirl(v.texcoord.xy);
           //v.vertex.y = length(worldPos.xy)*2. + swl.x*3.;
       }
        void surf (Input IN, inout SurfaceOutputStandard o)
        {
            // Albedo comes from a texture tinted by color
            fixed4 c = tex2D (_MainTex, IN.uv_MainTex) * _Color;
            fixed2 pos = ((IN.uv_MainTex.xy) - 0.5)*2.0; // origin is in center
            o.Albedo = swirl(pos);
            // Metallic and smoothness come from slider variables
            o.Metallic = _Metallic;
            o.Smoothness = _Glossiness;
            o.Alpha = c.a;
        }
        ENDCG
    }
    FallBack "Diffuse"
}
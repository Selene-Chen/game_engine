/*
 * @Author: Selene 339698418@qq.com
 * @Date: 2022-12-04 20:02:07
 * @LastEditors: Selene 339698418@qq.com
 * @LastEditTime: 2022-12-19 12:50:48
 * @FilePath: \game_engine\hazel\hazel\renderer\shader.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include "hazel/core/core.h"

namespace Hazel
{
    class Shader
    {
    public:
        Shader()          = default;
        virtual ~Shader() = default;

        virtual void Bind() const                                                       = 0;
        virtual void UnBind() const                                                     = 0;
        virtual void SetInt(const std::string& name, int value) const                   = 0;
        virtual void SetIntArray(const std::string& name, int* values, int count) const = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) const     = 0;
        virtual void SetFloat(const std::string& name, float value) const               = 0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) const   = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) const   = 0;

        [[nodiscard]] virtual const std::string& GetName() const = 0;

        static Ref<Shader> Create(const std::string& name, const std::string& vertex_src,
                                  const std::string& fragment_src);
        static Ref<Shader> Create(const std::string& path);
    };
    class ShaderLibrary
    {
    public:
        ShaderLibrary()          = default;
        virtual ~ShaderLibrary() = default;

        void Add(const std::string& name, const Ref<Shader>& shader);
        void Add(const Ref<Shader>& shader);

        Ref<Shader> Load(const std::string& filepath);
        Ref<Shader> Load(const std::string& name, const std::string& filepath);

        Ref<Shader> Get(const std::string& name);

        [[nodiscard]] bool Exists(const std::string& name) const;

    private:
        std::unordered_map<std::string, Ref<Shader>> m_shaders;
    };
}  // namespace Hazel
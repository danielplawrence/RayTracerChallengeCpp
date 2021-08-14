#pragma once

#include <string>

namespace raytracerchallenge {

  /**  Language codes to be used with the RayTracerChallenge class */
  enum class LanguageCode { EN, DE, ES, FR };

  /**
   * @brief A class for saying hello in multiple languages
   */
  class RayTracerChallenge {
    std::string name;

  public:
    /**
     * @brief Creates a new raytracerchallenge
     * @param name the name to greet
     */
    RayTracerChallenge(std::string name);

    /**
     * @brief Creates a localized string containing the greeting
     * @param lang the language to greet in
     * @return a string containing the greeting
     */
    std::string greet(LanguageCode lang = LanguageCode::EN) const;
  };

}  // namespace raytracerchallenge

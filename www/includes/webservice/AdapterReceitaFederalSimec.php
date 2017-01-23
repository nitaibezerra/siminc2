<?php

/**
 * Classe para converter o formato de dados do WebService da Receita com o formato em que o SIMEC aceita trabalhar.
 * 
 */
class AdapterReceitaFederalSimec {

    /**
     * Busca os dados de Pessoa Fisica por número de CPF informado.
     * 
     * @param string $numero
     * @return string
     */
    public static function solicitarDadosPessoaJuridicaPorCnpj($numero){
        $wsReceitaFederal = new RestReceitaFederal();
        $pj = $wsReceitaFederal->consultarPessoaJuridicaReceitaFederal($numero);
        $pfSimec = self::inserirCamposPJSimec(self::formatar($pj));
        
        $xmlDataObject = new SimpleXMLElement('<?xml version="1.0"?><document></document>');
        self::arrayToXml($pfSimec, $xmlDataObject);

        $xml = $xmlDataObject->asXML();
        
        return $xml;
    }
    
    /**
     * Busca os dados de Pessoa Fisica por número de CPF informado.
     * 
     * @param string $numero
     * @return string
     */
    public static function solicitarDadosPessoaFisicaPorCpf($numero){
        $wsReceitaFederal = new RestReceitaFederal();
        $pf = $wsReceitaFederal->consultarPessoaFisicaReceitaFederal($numero);
        $pfSimec = self::inserirCamposPFSimec(self::formatar($pf));
        
        $xmlDataObject = new SimpleXMLElement('<?xml version="1.0"?><document></document>');
        self::arrayToXml($pfSimec, $xmlDataObject);

        $xml = $xmlDataObject->asXML();
        
        return $xml;
    }
    
    /**
     * Converte array para XML.
     * 
     * @param array $data
     * @param SimpleXMLElement $xml_data
     */
    public static function arrayToXml($data, &$xml_data) {
        foreach($data as $key => $value) {
            if(is_numeric($key)){
                $key = 'item'. $key; //dealing with <0/>..<n/> issues
            }
            if(is_array($value) || is_object($value)){
                $subnode = $xml_data->addChild($key);
                self::arrayToXml((array)$value, $subnode);
            } else {
                $xml_data->addChild("$key", htmlspecialchars("$value"));
            }
         }
    }
    
    /**
     * Formata o resultado da consulta de Webservice para o formato utilizado no SIMEC.
     * 
     * @param stdclass $pf
     * @return array
     */
    public static function formatar(stdclass $pf){
        $resultado = array();
        
        if($pf){
            foreach($pf as $attr => $val){
                if(is_array($val)){
                    $resultado[strtoupper($attr)] = (object) self::formatarMaiusculasAtributo($val);
                } else {
                    $resultado[self::fromCamelcaseToUnderscore(self::formatarPreFixo($attr))] = $val;
                }
            }
        }

        return $resultado;
    }
    
    /**
     * Insere campos extras de Pessoa Fisica esperados nas funções de retorno do SIMEC.
     * 
     * @param array $resultado 
     * @return array
     */
    public static function inserirCamposPFSimec(array $resultado) {
        $resultado['PESSOA']->no_pessoa_rf = ucwords(strtolower($resultado['no_pessoa_fisica']));
        $resultado['PESSOA']->nu_cpf_rf = $resultado['nu_cpf'];
        $resultado['PESSOA']->no_mae_rf = ucwords(strtolower($resultado['no_mae']));
        $resultado['PESSOA']->dt_nascimento_rf = str_replace('-', '', trim(formatDateWS($resultado['dt_nascimento'])));
        $resultado['PESSOA']->sg_sexo_rf = trim($resultado['sg_sexo']);
        $resultado['PESSOA']->nu_titulo_eleitor_rf = trim($resultado['DOCUMENTACAO']->nu_titulo_eleitor);
        $resultado['PESSOA']->st_indicador_estrangeiro_rf = trim($resultado['PESSOA']->PAIS->co_ddi);
        $resultado['PESSOA']->co_pais_residente_exterior_rf = trim($resultado['PESSOA']->PAIS->sg_pais_iso);
        $resultado['PESSOA']->st_indicador_residente_ext_rf = trim($resultado['PESSOA']->PAIS->sg_pais_iso);
        $resultado['PESSOA']->st_cadastro_rf = trim($resultado['SITUACAOCADASTRAL']->dt_situacao_cadastral);
        $resultado['PESSOA']->nu_rg = trim($resultado['DOCUMENTACAO']->nu_rg);
        $resultado['PESSOA']->dt_emissao_rg = str_replace('-', '', trim(formatDateWS($resultado['DOCUMENTACAO']->dt_expedicao_rg))); 
        $resultado['PESSOA']->ds_orgao_expedidor_rg = trim($resultado['DOCUMENTACAO']->no_orgao_exp_rg);
        $resultado['PESSOA']->dt_cadastro = str_replace('-', '', trim(formatDateWS($resultado['SITUACAOCADASTRAL']->dt_situacao_cadastral)));
        $resultado['PESSOA']->ENDERECOS->ENDERECO->co_cidade = trim($resultado['PESSOA']->ENDERECOS->ENDERECO->id_endereco);
        $resultado['PESSOA']->ENDERECOS->ENDERECO->co_tipo_endereco_pessoa = trim($resultado['PESSOA']->ENDERECOS->ENDERECO->TIPOENDERECO->co_tipo_endereco);
        $resultado['PESSOA']->ENDERECOS->ENDERECO->sg_uf = trim($resultado['PESSOA']->ENDERECOS->ENDERECO->LOGRADOURO->UF->sg_uf);
        $resultado['PESSOA']->ENDERECOS->ENDERECO->ds_localidade = trim($resultado['PESSOA']->ENDERECOS->ENDERECO->LOGRADOURO->MUNICIPIO->no_municipio);
        $resultado['PESSOA']->ENDERECOS->ENDERECO->ds_bairro = trim($resultado['PESSOA']->ENDERECOS->ENDERECO->ds_bairro_endereco);
        $resultado['PESSOA']->ENDERECOS->ENDERECO->ds_logradouro = trim($resultado['PESSOA']->ENDERECOS->ENDERECO->LOGRADOURO->no_logradouro);
        $resultado['PESSOA']->ENDERECOS->ENDERECO->ds_logradouro_comp = trim($resultado['PESSOA']->ENDERECOS->ENDERECO->LOGRADOURO->ds_complemento);
        $resultado['PESSOA']->ENDERECOS->ENDERECO->ds_tipo_logradouro = trim($resultado['PESSOA']->ENDERECOS->ENDERECO->LOGRADOURO->ds_tipo_logradouro);
        $resultado['PESSOA']->ENDERECOS->ENDERECO->ds_numero = trim($resultado['PESSOA']->ENDERECOS->ENDERECO->nu_complemento);
        $resultado['PESSOA']->ENDERECOS->ENDERECO->nu_cep = trim($resultado['PESSOA']->ENDERECOS->ENDERECO->LOGRADOURO->nu_cep);
        
        return $resultado;
    }
    
    /**
     * Insere campos extras de Pessoa Fisica esperados nas funções de retorno do SIMEC.
     * 
     * @param array $resultado 
     * @return array
     */
    public static function inserirCamposPJSimec(array $resultado) {
        foreach($resultado as $nomeAtributo => $valor){
            $nomeAtributoSimec = $nomeAtributo. '_rf';
            if(!is_array($valor) && !is_object($valor)){
                $resultado['PESSOA']->$nomeAtributoSimec = $valor;
            }
        }
        # Tipo de Natureza Juridica
        $resultado['PESSOA']->no_empresarial_rf = ucwords(strtolower($resultado['NATUREZAJURIDICA']->ds_natureza_juridica));
        $resultado['PESSOA']->co_natureza_juridica_rf = $resultado['NATUREZAJURIDICA']->co_natureza_juridica;
        # Contatos
        $resultado['PESSOA']->CONTATOS = new stdClass();
        $resultado['PESSOA']->CONTATOS->CONTATO = array();
        $listaTelefones = $resultado['PESSOA']->TELEFONES;
        foreach($listaTelefones as $telefone){
            $telefone->ds_contato_pessoa = $telefone->DDD->co_ddd. '-'. $telefone->nu_telefone;
            $resultado['PESSOA']->CONTATOS->CONTATO[] = $telefone;
        }
        
        return $resultado;
    }

    /**
     * Formata nomes das chaves que são sublistas em letras maiusculas.
     * 
     * @param array $atributo
     * @return array
     */
    public static function formatarMaiusculasAtributo($atributo){
        $resultado = array();
        
        if(is_array($atributo)){
            foreach($atributo as $attr => $val){
                if(is_array($val)){
                    switch (strtoupper($attr)) {
                        case 'ENDERECOS':
                            $resultado['ENDERECOS'] = new stdClass();
                            $resultado['ENDERECOS']->ENDERECO = (object) end(self::formatarMaiusculasAtributo($val));
                        break;
                        default:
                            $resultado[strtoupper($attr)] = (object) self::formatarMaiusculasAtributo($val);
                        break;
                    }
                } else {
                    $resultado[self::fromCamelcaseToUnderscore(self::formatarPreFixo($attr))] = $val;
                }
            }
        }
        
        return $resultado;
    }
    
    /**
     * Formata prefixo para o formato padrão utilizado no SIMEC.
     * 
     * @param string $texto
     * @return string
     */
    public static function formatarPreFixo($texto){
        $resultado = NULL;
        
        $prefixo = substr($texto, 0, 2);
        switch ($prefixo) {
            case 'nm':
                $resultado = 'no'. substr($texto, 2);
            break;
            case 'nr':
                $resultado = 'nu'. substr($texto, 2);
            break;
            case 'cd':
                $resultado = 'co'. substr($texto, 2);
            break;
            default:
                $resultado = $texto;
        }
        
        return $resultado;
    }
    
    /**
     * Formata o nome do formato Camelcase para Underscore.
     * 
     * @param string $input
     * @return string
     */
    public static function fromCamelcaseToUnderscore($input) {
        preg_match_all('!([A-Z][A-Z0-9]*(?=$|[A-Z][a-z0-9])|[A-Za-z][a-z0-9]+)!', $input, $matches);
        $ret = $matches[0];
        foreach ($ret as &$match) {
            $match = $match == strtoupper($match) ? strtolower($match) : lcfirst($match);
        }
        
        return implode('_', $ret);
    }

}
<?php
/**
 * 
 */

/**
 *
 * @abstract
 * @todo Incluir no log de chamadas o usuário que originou a chamada.
 *
 */
abstract class Simec_SoapClient_Log_Logger
{
    /**
     * Tipo da classe de log.
     * @var string
     */
    protected $loggerType;

    /**
     * Mapeamento de campos de log.
     *
     * @see Simec_SoapClient_Log_Logger::addField()
     * @var array
     */
    protected $fieldMap = array();

    /**
     * Faz o mapeamento de campos de log com as informações que eles irão armazenar.
     *
     * @param string $name Identificação do campo na estrutura de log.
     * @param string $map Mapeamento do campo na escrita do log (título do campo, ou campo da tabela, por exemplo).
     * @return \Simec_SoapClient_Log_Logger
     */
    public function addField($name, $map)
    {
        $this->fieldMap[$name] = $map;
        return $this;
    }

    /**
     * Retorna o tipo de log da classe filha.
     * @return string
     */
    public function getLoggerType()
    {
        return $this->loggerType;
    }

    /**
     * Gera um id para a requisição.
     * Usado quando o ID da requisição não é gerado automaticamente.
     *
     * @return string
     */
    public function generateRequestID()
    {
        return base64_encode(pack('H*', mt_rand() . time()));
    }

    /**
     * Recebe as configurações do logger e as carrega.
     * 
     * @param array $configData
     *       Array associativo com dados de configuração do funcionamento do logger.
     */
    abstract public function setConfig(array $configData);

    /**
     * Grava o log da requisição.
     *
     * @param array $data Array associativo com os dados para inserção no log.
     * @return mixed Retorno do webservice para a requisição.
     * @abstract
     */
    abstract public function writeLog(array $data);
}

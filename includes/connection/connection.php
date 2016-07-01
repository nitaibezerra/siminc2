<?php
/**
 * Implementação da classe connection.
 *
 * @version $Id$
 * @filesource
 */

/**
 * Classe para estabelecer conexão e permitir manipular o banco de dados adicionais no sistema.
 *
 * @example
 * <code>
 * $listaResultado = connection::getInstance()
 *     ->setHost($config->host)
 *     ->setPort($config->port)
 *     ->setUser($config->user)
 *     ->setPassword($config->password)
 *     ->setDbname($config->dbname)
 *     ->connect()
 *     ->fetch("SELECT u.usunome FROM seguranca.usuario u WHERE u.usucpf = ''");
 * </code>
 *
 * @package Simec\Db
 * @author Rafael Jose da Costa Gloria <RafaelGloria@mec.gov.br>
 */
class connection {

    /**
     * Nome do servidor(Host) do banco de dados
     *
     * @var string
     */
    private $host;

    /**
     * Numero da porta para conexão com banco de dados
     *
     * @var integer
     */
    private $port;

    /**
     * Nome do banco de dados
     *
     * @var string
     */
    private $dbname;

    /**
     * Nome de usuário para acesso ao banco de dados
     *
     * @var string
     */
    private $user;

    /**
     * Senha para acesso ao banco de dados
     *
     * @var string
     */
    private $password;

    /**
     * Configuracao search_path
     *
     * @var string
     */
    private $searchPath;

    /**
     * Configuração de client encoding
     *
     * @var string
     */
    private $clientEncoding;

    /**
     * Recurso de banco
     *
     * @var resource
     */
    private $link;

    /**
     * Resultado do commando executado no banco de dados
     *
     * @var mix
     */
    private $result;

    /**
     * Guarda uma instância da classe
     *
     * @var /connection
     */
    private static $instance;

    public function getHost() {
        return $this->host;
    }

    public function getPort() {
        return $this->port;
    }

    public function getDbname() {
        return $this->dbname;
    }

    public function getUser() {
        return $this->user;
    }

    public function getPassword() {
        return $this->password;
    }

    public function getSearchPath() {
        return $this->searchPath;
    }

    public function getClientEncoding() {
        return $this->clientEncoding;
    }

    public function getLink() {
        return $this->link;
    }

    public function getResult() {
        return $this->result;
    }

    /**
     * Atribui Host
     *
     * @param string $host
     * @return \connection
     */
    public function setHost($host) {
        $this->host = $host;
        return $this;
    }

    /**
     * Atribui Port
     *
     * @param integer $port
     * @return \connection
     */
    public function setPort($port) {
        $this->port = $port;
        return $this;
    }

    /**
     * Atribui dbname
     *
     * @param string $dbname
     * @return \connection
     */
    public function setDbname($dbname) {
        $this->dbname = $dbname;
        return $this;
    }

    /**
     * Atribui user
     *
     * @param string $user
     * @return \connection
     */
    public function setUser($user) {
        $this->user = $user;
        return $this;
    }

    /**
     * Atribui password
     *
     * @param string $password
     * @return \connection
     */
    public function setPassword($password) {
        $this->password = $password;
        return $this;
    }

    /**
     * Atribui search_path
     *
     * @param string $searchPath
     * @return \connection
     */
    public function setSearchPath($searchPath) {
        $this->searchPath = $searchPath;
        return $this;
    }

    /**
     * Atribui a configuração client encondig
     *
     * @param string $clientEncoding
     * @return \connection
     */
    public function setClientEncoding($clientEncoding) {
        $this->clientEncoding = $clientEncoding;
        return $this;
    }

    /**
     * Atribui o recurso de connection
     *
     * @param resource $link
     */
    public function setLink($link) {
        $this->link = $link;
    }

    /**
     * Atribui o resultado do comando executado no banco de dados
     *
     * @param boolean\array $result
     */
    public function setResult($result) {
        $this->result = $result;
    }

    /**
     * Retorna a instancia da classe. Caso não exista instancia o metodo cria uma nova instancia.
     *
     * @example $objConnection = connection::getInstance();
     * @return \connection
     */
    public static function getInstance(){
        if (!isset(self::$instance)){
            $className = __CLASS__;
            self::$instance = new $className;
        }

        return self::$instance;
    }

    /**
     * Estabelece conexão e permite manipular banco de dados adicionais no sistema
     *
     * @example connection::getInstance();
     * @return VOID
     */
    private function __construct() {}

    /**
     * Atribui search_path depedendo do host(servidor) que foi realizada conexão
     *
     * @return VOID
     */
    private function loadSearchPath(){
        if(!empty($this->searchPath)){
            pg_query($this->link, $this->searchPath);
        }
    }

    /**
     * Atribui o cliente enconding
     *
     * @return VOID
     */
    private function loadClientEnconding(){
        if(!empty($this->clientEncoding)){
            pg_set_client_encoding($this->link, $this->clientEncoding);
        }
    }

    /**
     * Realiza a conexão do banco e retorna o objeto de conexão ou mensagem de erro
     *
     * @return boolean
     */
    public function connect(){
        # Verifica se existe conexão pra encerrar antes de realizar uma nova conexão
        $this->close();

        # Abre conexão com o banco
        $this->link = pg_connect(
            "host=".$this->host.
            " port=".$this->port.
            " dbname=".$this->dbname.
            " user=".$this->user.
            " password=".$this->password);

        # Verifica se a conexão foi realizada com sucesso
        if((!$this->link)){
            $this->result = FALSE;
        } else {
            # Atribui exceção depedendo do host(servidor) que foi realizada conexão
            $this->loadSearchPath();

            # Configura encode
            $this->loadClientEnconding();

            $this->result = TRUE;
        }
        return $this->result;
    }

    /**
     * Utilizado para executar SCRIPTS(select) no banco de dados com objetivo de retornar resultados
     *
     * @description Retorna os dados da consulta sql. / Atribui o resultado da consulta a uma variável global.
     * @param string (obrigatório) Consulta SQL
     * @param string var (opcional)
     * @return array/null
     * @author Rafael J
     * @since 23/02/2015
     */
    public function fetch($sql, $var = NULL, $timeCache = NULL) {
        if($_SERVER['HTTP_HOST'] == 'pdeinterativo-local') $timeCache=NULL;
        if($timeCache) {

            # Pegando informações do memcached server, key igual ao md5 do SQL */
            //$cache_result = memcache_get($memcache_obj, md5($sql));
        	if (function_exists('zend_shm_cache_fetch')) {$cache_result = zend_shm_cache_fetch(md5($sql));}

            # Se existir cache, carregar com o resultado do memcached server
            if($cache_result) {
                $this->result = $cache_result;
            } else {
                # Senão executa o SQL e guarda o resultado no memcached server
                $this->result = pg_fetch_all($this->execute($sql));
                # Armazenando os dados memcached server na chave md5(SQL), 0 => sem compressão, tempo para expirar de 30 seconds
                //memcache_set($memcache_obj, md5($sql), $this->result, 0, $timeCache);
                if (function_exists('zend_shm_cache_store')) {
                	if(zend_shm_cache_store(md5($sql), $this->result, $timeCache) === false) echo '[ZEND CACHE FALHOU]';
                }
                
            }
        } else {
            $this->result = pg_fetch_all($this->execute($sql));
        }

        if($var != NULL){
            global ${$var};
            ${$var} = $this->result;
        }

        return $this->result;
    }

    /**
     * Busca todos os dados de um registro
     *
     * @global object $memcache_obj
     * @param string $sql
     * @param integer $numberRow
     * @param integer $timeCache
     * @return boolean/array
     */
    public function fetchRow($sql, $numberRow = 0, $timeCache = NULL) {
        if($_SERVER['HTTP_HOST'] == 'pdeinterativo-local') $timeCache=NULL;
        if($timeCache) {
            # Pegando informações do memcached server, key igual ao md5 do SQL
            //$cache_result = memcache_get($memcache_obj, md5($sql));
        	if (function_exists('zend_shm_cache_fetch')) {$cache_result = zend_shm_cache_fetch(md5($sql));}

            if($cache_result) {
                # Se existir cache, carregar com o resultado do memcached server
                $this->result = $cache_result;
            } else {
                # Senão executa o SQL e guarda o resultado no memcached server
                if(($recordSet = $this->execute($sql)) && (pg_num_rows($recordSet)>=1)) {
                    $this->result = pg_fetch_assoc($recordSet, $numberRow);
                    # Armazenando os dados memcached server na chave md5(SQL), 0 => sem compressão, tempo para expirar de 30 seconds
                    if (function_exists('zend_shm_cache_store')) {
                    	if(zend_shm_cache_store(md5($sql), $this->result, $timeCache) === false) echo '[ZEND CACHE FALHOU]';
                    }
                }
            }
        } else {
            # Retorna um registro de uma query, a partir da coluna especificada
            if(($recordSet = $this->execute($sql)) && (pg_num_rows($recordSet)>=1)) {
                $this->result = pg_fetch_assoc($recordSet, $numberRow);
            }
        }

        return $this->result;
    }

    /**
     * Busca uma coluna do registro consultado
     *
     * @global object $memcache_obj
     * @param string $sql
     * @param integer $numberRow
     * @param integer $timeCache
     * @return boolean/string
     */
    function fetchOne($sql, $numberRow = 0, $timeCache = NULL) {
        if($_SERVER['HTTP_HOST'] == 'pdeinterativo-local') $timeCache = NULL;
        if($timeCache) {
            # Pegando informações do memcached server, key igual ao md5 do SQL
        	if (function_exists('zend_shm_cache_fetch')) {$cache_result = zend_shm_cache_fetch(md5($sql));}
            if($cache_result) {
                # Se existir cache, carregar com o resultado do memcached server
                $this->result = $cache_result;
            } else {
                # Senão executa o SQL e guarda o resultado no memcached server
                if(($recordSet = $this->execute($sql)) && (pg_num_rows($recordSet)>=1)) {
                    $this->result = pg_fetch_result($recordSet, 0, $numberRow);
                    # Armazenando os dados memcached server na chave md5(SQL), 0 => sem compressão, tempo para expirar de 30 seconds
                	if (function_exists('zend_shm_cache_store')) {
						if(zend_shm_cache_store(md5($sql), $this->result, $timeCache) === false) echo '[ZEND CACHE FALHOU]';
					}
                    
                }
            }
        } else {
            # Retorna um registro de uma query, a partir da coluna especificada
            if(($recordSet = $this->execute($sql)) && (pg_num_rows($recordSet)>=1)) {
                $this->result = pg_fetch_result($recordSet, 0, $numberRow);
            }
        }

        return $this->result;
    }

    /**
     * Retorna a string de resultado da execução do SQL. / Cria um log de auditoria no servidor externo.
     *
     * @todo Adicionar Auditoria e verificar viabilidade da transação automatica
     * @author Rafael J
     * @since 19/02/2015
     * @param string $sql Script de banco (obrigatório)
     * @param boolean $manipulation (opcional) Permiti executar INSERT, UPDATE, DELETE
     * @return mix
     */
    function execute($sql, $manipulation = FALSE){
//        if (!is_resource($this->link)){
//            throw new Exception("Não foi possível estabelecer uma conexão com o
//                Banco de Dados para executar o seguinte SQL: <br />".$SQL);
//        }

        $sql = trim($sql);

        # Detecta operacao de tabela (DML - Insert, Update ou Delete)
        $matches = array();
        preg_match(
            '/(CREATE\s+TABLE|ALTER\s+TABLE|DROP\s+TABLE|SELECT.*FROM|INSERT\s+INTO|UPDATE|DELETE\s+FROM)\s+([A-Za-z0-1.]+).*/smui',
            utf8_encode($sql),
            $matches);
        $audtipoCompleto = strtoupper($matches[1]);
        $audtipo = substr($audtipoCompleto, 0, 1);
        # Faz critica caso exista alteração em estrutura das tabelas
        if ($audtipoCompleto == 'ALTER TABLE' || $audtipoCompleto == 'DROP TABLE' || ($manipulation === FALSE && $audtipo != 'S' ) ){
            throw new Exception('A execução do comando "' .$audtipoCompleto. '" não é permitido pelo sistema!');
        }

//        $momentoAnterior = microtime();
        $this->result = pg_query($this->link, $sql);
//        $momentoPosterior = microtime();
//        # Exibi log de execucao no FirePHP
//        $this->salvaQueryArray($SQL, $momentoAnterior, $momentoPosterior);

//        if ($this->result == NULL){
//            throw new Exception($SQL.pg_errormessage($this->link));
//        }

        return $this->result;
    }

    /**
     * Inicia transação para os comandos a serem realizados no banco de dados
     *
     * @return VOID
     */
    public function begin(){
        if (is_resource($this->link)){
            pg_query($this->link, 'BEGIN TRANSACTION; ');
        }
    }

    /**
     * Cancela comandos realizados na transação do banco de dados
     *
     * @return VOID
     */
    public function rollback(){
        if (is_resource($this->link)){
            pg_query($this->link, 'ROLLBACK; ');
        }
    }

    /**
     * Efetua ações realizadas realizadas na transação do banco de dados
     *
     * @return boolean
     */
    public function commit(){
        if (is_resource($this->link)){
            pg_query($this->link, 'COMMIT; ');
            return TRUE;
        } else {
            return FALSE;
        }
    }

    /**
     * Encerra a conexão com o Banco de Dados.
     *
     * @return VOID
     */
    public function close(){
        if (is_resource($this->link)){
            $this->rollback();
            pg_close($this->link);
        }
    }

    /**
     * Ao descarregar o objeto da mémoria encerra a conexão com o Banco de Dados.
     *
     * @return VOID
     */
    public function __destruct(){
        $this->close();
    }

    /**
     * Impede que o desenvolvedor clone ou copie a instância evitando várias conexões abertas
     * em um mesmo escopo do programa
     *
     * @return VOID
     */
    public function __clone()
    {
        trigger_error(
            'Não é permitido clonar ou copiar a instância da classe de conexão!',
            E_USER_ERROR);
    }

}

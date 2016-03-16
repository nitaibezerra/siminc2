<?php
/**
 * Classe de mapeamento da entidade par3.instrumentounidade_entidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-09-2015
 * @link     no link
 */

/**
 * Par_Modelo_InstrumentoUnidadeEntidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-09-2015
 * @link     no link
 */
class Par3_Model_InstrumentoUnidadeEntidade extends Modelo
{
    /**
     * Nome da tabela especificada
     *
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.instrumentounidade_entidade';

    /**
     * Chave primaria.
     *
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'entid',
    );

    /**
     * Chaves estrangeiras.
     *
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'endid' => array('tabela' => 'par3.endereco', 'pk' => 'endid'),
        'tenid' => array('tabela' => 'par3.tipoentidade', 'pk' => 'tenid'),
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
    );

    /**
     * Atributos
     *
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'entid' => null,
        'inuid' => null,
        'tenid' => null,
        'endid' => null,
        'entcpf' => null,
        'entnome' => null,
        'entemail' => null,
        'enttelefonecomercial' => null,
        'enttelefonecelular' => null,
    	'enttelefonefax' => null,
    	'entcnpj' => null,
        'entinscricaoestadual' => null,
        'entrazaosocial' => null,
        'entsigla' => null,
    	'entstatus' => null,
    	'entrg' => null,
    	'entorgexpedidor' => null,
    	'entdtnascimento' => null,
    	'entnivelensino' => null,
    	'entempoatuacao' => null,
    	'entcursomec' => null,
    	'entcursomecdescricao' => null,
        'entdtinativacao' => null,
   		'entatuacao' => null,
   		'entcargo' => null,
    		
    );

	const PREFEITURA                   = 1;
	const PREFEITO                     = 2;
	const SECRETARIA_EDUCACAO          = 3;
	const DIRIGENTE                    = 4;
	const EQUIPE                       = 5;
	const SECRETARIO_ESTADUAL_EDUCACAO = 9;
    const NUTRICIONISTA_RESPONSAVEL    = 7;
    const NUTRICIONISTA_QUADRO         = 8;
    const CONSELHO_ESTADUAL	           = 10;
    const CONSELHO_MUNICIPAL	       = 11;
    

    /**
     * Função pegarEntidAtivoPorTipo
     * - retorna o entid ativo.
     *
     * @return integer entid.
     *
     */
    public function pegarEntidAtivoPorTipo($inuid, $tenid)
    {
    	$sql = "SELECT entid
    	        FROM {$this->stNomeTabela}
    	        WHERE
    	           inuid = $inuid
        	       AND entstatus = 'A'
        	       AND tenid = $tenid";

    	return $this->pegaUm($sql);

    }//end pegarEntidAtivoPorTipo()


    /**
     * Função carregarDadosEntidPorTipo
     * - retorna o dados da entidade por id.
     *
     * @return retorna um objeto dirigente.
     *
     */
    public function carregarDadosEntidPorTipo($inuid, $tenid)
    {
    	$entid = $this->pegarEntidAtivoPorTipo($inuid, $tenid);

		$objEntidade = new Par3_Model_InstrumentoUnidadeEntidade($entid);

        return $objEntidade;
    }//end carregarDadosEntidPorTipo()


    /**
     * Função carregaArrayHistoricoEntidade
     * - retorna array de entidades.
     *
     * @return array $arrDados.
     *
     */
    public function carregaArrayHistoricoEntidade($arrPost)
    {
        $sql = self::pegarSQLHistoricoEntidade($arrPost);

        $arrDados = self::carregar($sql);

        return $arrDados;

    }//end carregaArrayHistoricoEntidade()


    /**
     * Função pegarSQLHistóricoEntidade
     * - retorna sql para histórico de entidade.
     *
     * @return string $sql.
     *
     */
    public function pegarSQLHistoricoEntidade($arrPost)
    {
        $where = self::montarFiltroHistoricoEntidade($arrPost);

        $sql = "SELECT	coalesce(entcpf, entcnpj) as cadastro, *
                FROM 	par3.instrumentounidade_entidade
                WHERE	".implode(' AND ', $where);

        return $sql;

    }//end pegarSQLHistoricoEntidade()


    /**
     * Função montarFiltroHistóricoEntidade
     * - retorna filtro para histórico de entidade.
     *
     * @return string $sql.
     *
     */
    public function montarFiltroHistoricoEntidade($arrPost)
    {
        $where = array();
        if ($arrPost['inuid'] != '') $where[]     = "inuid = {$arrPost['inuid']}";
        if ($arrPost['tenid'] != '') $where[]     = "tenid = {$arrPost['tenid']}";
        if ($arrPost['entstatus'] != '') $where[] = "entstatus = '{$arrPost['entstatus']}'";

        return $where;

    }//end montarFiltroHistoricoEntidade()


    /**
     * Função inativaEntidadesInuidPorTipo
     * - intaiva entidades da unidade por tipo.
     *
     * @return void.
     *
     */
    public function inativaEntidadesInuidPorTipo($inuid, $tenid)
    {
        $sql = "UPDATE 	{$this->stNomeTabela} SET
                    	entstatus = 'I',
                    	entdtinativacao = now()
                WHERE 	inuid = $inuid
                  AND	tenid = $tenid";

        $this->executar($sql);
    }//end inativaEntidadesInuidPorTipo()

    
    /**
     * Função existeConselheiro
     * - validar se o cpf já possui cadastro como conselheiro.
     *
     * @return void.
     *
     */
    public function existeConselheiro($usucpf, $inuid) 
    {
    	$sql = "SELECT  entid
    			FROM 	{$this->stNomeTabela}
    		   WHERE 	entcpf = '{$usucpf}' 
    		     AND 	entstatus = 'A'
    		     AND 	inuid = '{$inuid}'
    			 AND	tenid IN (" . self::CONSELHO_ESTADUAL . "," . self::CONSELHO_MUNICIPAL . ")";
    	
    	return $this->carregar($sql);
    }//end existeConselheiro()
    
    
    /**
     * Função carregarConselheiros
     * - retorna array de entidades do tipo conselheiro.
     *
     * @return array $arrDados.
     *
     */
    public function carregarConselheiros($inuid)
    {
    	$sql = "SELECT  *
    			FROM 	{$this->stNomeTabela}
    		   WHERE 	inuid = '{$inuid}'
    		   	 AND	entstatus = 'A'
    			 AND	tenid IN (" . self::CONSELHO_ESTADUAL . "," . self::CONSELHO_MUNICIPAL . ")";

    	return $this->carregar($sql);
    }//end carregarConselheiros()
    
    
    public function recuperarNutricionistaResponsavelPorCpf($usucpf)
    {
        $sql = "
                select usunome,entemail,inudescricao,estuf,entemail,'instrumentounidade_entidade' as origem
                from {$this->stNomeTabela}  int
                join seguranca.usuario usu on int.entcpf = usu.usucpf
                join par3.instrumentounidade ine on int.inuid = ine.inuid
                where int.entcpf = '{$usucpf}' and int.tenid = ".self::NUTRICIONISTA_RESPONSAVEL;
        
        return $this->carregar($sql);
    }

    
    public function recuperarNutricionistasQuadroTecnico()
    {
        $sql = "
                select int.entid as id,entcpf, entnome, entemail
                from {$this->stNomeTabela}  int
                join seguranca.usuario usu on int.entcpf = usu.usucpf
                join par3.instrumentounidade ine on int.inuid = ine.inuid
                where int.tenid = ".self::NUTRICIONISTA_QUADRO;
        
        return $sql;
    }
}//end Class
?>
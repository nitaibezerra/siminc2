<?php
	
class PseAnoEscolaMun extends Modelo{
	
	public function __construct($pamid,$muncod = null ){
		
		parent::__construct();
		
		if( !$pamid && $muncod )
		{
			/*
			$pamid = $this->pegaUm( "SELECT 
										pam.pamid
									FROM
										pse.pseanoescolamun AS pam
									
									INNER JOIN pse.estadomunicipiopse AS emp ON emp.empid = pam.empid 	
									WHERE		
										emp.muncod ='{$muncod}'" );
		*/
			$pamid = '';
		}
		
		if( $pamid ){
			$this->carregarPorId( $pamid );
		//	$this->carregarSCNES();
		}
	}	
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.pseanoescolamun";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pamid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pamid' => null, 
									  	'pamanoreferencia' => null, 
									  	'empid' => null, 
									  	'esfid' => null, 
									  	'moeid' => null, 
									  	'nieid' => null, 
									  	'pamquantprevista' => null, 
									  	'pamquantatendida' => null,
    									'entid' => null, 
									  );
	protected $cneid;
									  
	function carregaListaEscolas($empid = null)
	{
		if(!$this->pamanoreferencia)
			$this->pamanoreferencia = 'null';
			
		 $sql = "SELECT
					 '<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarFormulario('||pam.pamid||',\'pse2009\');\" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
					 '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiFormulario('||pam.pamid||',\'pse2009\');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' as acao,
					 scnes.cnecodigocnes, scnes.cneseqequipe, ent.entnome,  moe.moedsc, nie.niedsc, pam.pamquantprevista , pam.pamquantatendida
				FROM 
					pse.pseanoescolamun pam
					
				INNER JOIN pse.modalidadeensino AS moe ON moe.moeid = pam.moeid
				INNER JOIN pse.nivelensino AS nie ON nie.nieid = pam.nieid
				INNER JOIN entidade.entidade AS ent ON ent.entid = pam.entid 
				INNER JOIN pse.scnes AS scnes ON scnes.cneid = pam.esfid 
				 
				WHERE
					pam.empid = {$empid} AND
					pam.pamanoreferencia = {$this->pamanoreferencia}
				ORDER BY pam.pamid ";
		$cabecalho = array("Opções","ESF","N° Equipe", "Escola", "Modalidade", "Nível", "Previsto", "Realizado");
		//$alinha = array("center","center","center","left","left");
		//$tamanho = array("5%","10%","10%","30%","45%");
		$this->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '');
		 
		 //$this->monta_lista_grupo($sql, $cabecalho, 10, 5, 'N','Center','','formListaGrupo');
	}	

	function carregarSCNES()
	{
		$sql = "SELECT cneid FROM pse.equipesaudefamilia WHERE esfid = {$this->esfid}";
		
		$this->cneid =  $this->pegaUm($sql);
		
	}
	
	function carregarSNES()
	{
		$sql = "SELECT	
					esf.esfid as codigo,
					sc.cnecodigocnes||' - '||'Equipe '||sc.cneseqequipe||' - '||sc.cnenomefantasia as descricao
				FROM	
					pse.equipesaudefamilia esf
				INNER JOIN pse.pseanoescolamun AS pam ON pam.esfid = esf.esfid
				INNER JOIN pse.scnes AS sc ON esf.cneid = sc.cneid
				WHERE
					pam.pamid = $this->pamid";
					
		$this->monta_combo( "cneid", $sql, 'S', 'Selecione...', 'cneid', '', '', '215', 'S','cneid','',$this->esfid);
	}
	
	function identificaESF($entid)
	{
		if($entid == 0)
		{
			if($this->espid)
			{
				$sql = "SELECT entid FROM pse.escolapse WHERE espid = {$this->espid}";
				$entid = $this->pegaUm($sql);
			}		
		}
		
		if(!$entid)
			$entid = 0;
		
		$sql = "SELECT	esf.esfid as codigo, sc.cnecodigocnes||' - '||'Equipe '||sc.cneseqequipe||' - '||sc.cnenomefantasia as descricao
				FROM	pse.equipesaudefamilia esf, pse.scnes sc, pse.escolapse es
				WHERE	esf.cneid = sc.cneid and esf.espid=es.espid and es.entid = $entid";

		
		$this->monta_combo( "esfid", $sql, 'S', 'Selecione...', '', '', '', '215', 'S','esfid','',$this->esfid,'código ESF');
	}
	
	function verificaConflito()
	{
		
		$sql = "SELECT
					pamid
				FROM
					{$this->stNomeTabela}
				WHERE
					pamanoreferencia = {$this->pamanoreferencia} AND
					esfid = {$this->esfid} AND
					moeid = {$this->moeid} AND
					nieid = {$this->nieid}";
		
		$rs = $this->carregar($sql);

		if($rs)
			return true;
		else
			return false;
	}
									  
							  
}
<?php
	
class Processos extends Modelo{
	
	/**
     * CONSTANTES DO PROCESSO PARA IDENTIFICAR QUAL TIPO DE EXECUÇÃO.
     * @const
     * @access protected
     */

	const SUBACOES_GENERICO_PAR		= 1;
	const OBRAS_PAR 				= 2;
	const OBRAS_PAC 				= 3;
	const SUBACOES_PAR_EMENDAS 		= 4;
	/*const SUBACOES_PAR_BRASILPRO 	= 5;
	const OBRAS_PAR_BRASILPRO 		= 6;
	const SUBACOES_BRASILPRO 		= 7;
	const OBRAS_BRASILPRO 			= 8;
	const OBRAS_EMENDAS 			= 9;
	const GENERICO_EMENDAS 			= 10;*/

	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "execucaofinanceira.processos";	
    protected $tipoExecucao = NULL;
    protected $tela 		= NULL; // Tela que esta carregando o processo - pode ser tela de Empenho ou pagamento
    protected $inuid 		= NULL;
    protected $esfera 		= NULL;
    protected $sisid 		= NULL;

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "proid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'proid' => null, 
									  	'pronumeroprocesso' => null, 
									  	'procnpj' => null, 
									  	'proesfera' => null, 
									  	'protipoprocesso' => null, 
									  	'probanco' => null, 
									  	'proagencia' => null, 
									  	'proconta' => null, 
									  	'prosequencialcontasigef' => null, 
									  	'pronumeroconvenio' => null, 
									  	'prodatainclusao' => null, 
									  	'prodocumenta' => null, 
									  	'pronumerosiafi' => null, 
									  	'proano' => null, 
									  	'muncod' => null, 
									  	'estuf' => null, 
									  	'usucpf' => null, 
									  	'tipid' => null, 
									  	'resid' => null, 
									  );
								  
	function __construct() {
		parent::__construct();
	}

									  
	public function listaprocessos($post = NULL){
		$cabecalho = Array('Ação','Numero do Processo','Nº no sistema Documenta','UF','Código IBGE', 'Município', 'Data de Inclusão','Processo do Tipo');
		
		$where = array();
		if($post['filtroGeral']){
			$filtros = $post['filtroGeral']; 
			$where[]= " ( 	
							removeacento(p.pronumeroprocesso) ilike removeacento(('%{$filtros}%'))
							OR removeacento(lower(p.prodocumenta)) ilike ('%{$filtros}%')
							OR removeacento(lower(p.estuf)) ilike ('%{$filtros}%')
							OR removeacento(p.muncod) ilike ('%{$filtros}%')
							OR removeacento(lower(m.mundescricao)) ilike removeacento(('%{$filtros}%'))
						)	
					";
		}
		
		if($post['requisicao'] == 'pesquisar') {		
			if($post['numeroprocesso']) 		$where[] = "p.pronumeroprocesso = '".$post['numeroprocesso']."'";
			if($post['municipio']) 				$where[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$post['municipio']."%')";
			if($post['estuf']) 					$where[] = "p.estuf='".$post['estuf']."'";
			if($post['empenhado'] == "1") 		$where[] = " p.pronumeroprocesso in (select distinct empnumeroprocesso from par.empenho where empnumeroprocesso = p.pronumeroprocesso) ";  //"(SELECT sum(ep2.empvalorempenho) FROM par.empenho ep2 WHERE ep2.empnumeroprocesso=p.pronumeroprocesso) > 0";
			if($post['empenhado'] == "2") 		$where[] = " p.pronumeroprocesso not in (select distinct empnumeroprocesso from par.empenho where empnumeroprocesso = p.pronumeroprocesso) "; // "(SELECT sum(ep2.empvalorempenho) FROM par.empenho ep2 WHERE ep2.empnumeroprocesso=p.pronumeroprocesso) = 0";
			if($post['tipoExecucao'] == "C")	$where[] = "p.protipoexecucao = 'C'";
			if($post['tipoExecucao'] == "T") 	$where[] = "p.protipoexecucao = 'T'";
			
			if($post['esfera'] == "e") {
				$where[] = "p.proesfera = 'E' ";
			} 
			if($post['esfera'] == "m") {
				$where[] = "p.proesfera = 'M' ";
			}
	
			if($post['anoprocesso']) $where[] = "p.proano = '".$post['anoprocesso']."'";
		}
		
		$sql = "SELECT 	'<center> 
							<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"visualizarProcesso('|| p.proid ||')\"><img title=\"Visualizar todo o processo.\" style=\"width: 18px; height: 18px;\" src=\"/imagens/livro.gif\" border=0 title=\"Selecionar\"></a> 
							<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"editarProcesso('|| p.proid ||')\"><img title=\"Alterar dados do processo.\" src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
							<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"excluirProcesso('|| p.proid ||')\"><img title=\"Excluir processo.\" src=\"/imagens/excluir.gif\" border=0 title=\"Selecionar\"></a>
						</center>' as acao, 
						p.pronumeroprocesso, p.prodocumenta, p.estuf, p.muncod, m.mundescricao, to_char(p.prodatainclusao, 'DD/MM/YYYY') as data,  tp.tipdescricao
				FROM execucaofinanceira.processos p
					inner join execucaofinanceira.tipoprocesso tp on tp.tipid = p.tipid
					left join territorios.municipio m ON m.muncod = p.muncod
				WHERE 1=1 ".($where ? " and ".implode(" and ", $where) : '');

		return $this->monta_lista( $sql, $cabecalho, 50, 20, 'N', 'center','N');
	}
	
	public function carregaDadosProcessoPorMuncodUF($muncod = NULL, $uf = NULL){
		$where = " WHERE ";
		if($muncod){
			$where .= " m.muncod = '".$muncod."'";
		}
		if($uf){
			$where .= " p.estuf = ".$uf;
		}
		
		$sql = "SELECT CASE WHEN p.estuf is null THEN m.estuf ELSE p.estuf END AS estuf, 
					CASE WHEN p.proesfera = 'M' THEN 'Processo Municipal' ELSE 'Processo Estadual' END AS esfera,
					p.muncod, 
					m.mundescricao
				FROM execucaofinanceira.processos p
				LEFT JOIN territorios.municipio m ON m.muncod = p.muncod
				".$where;

		return $this->pegaLinha($sql);

	}
	
	public function controleDeTela($tela){
		// ------------- REGRAS DE NEGÓCIO PARA CARREGAMENTO DE TELA (CONTROLE DE FLUXO) ------------- //
		/*$arrTipoProcesso = $this->getTipoProcesso();
		$tpdid		 		= $arrTipoProcesso['tpdid'];
		$this->sisid 		= $arrTipoProcesso['sisid'];
		$this->tipoExecucao	= $arrTipoProcesso['tpdid'];*/
		$this->tela 		= $tela;
		
		// CARREGA A ESFERA SE É ESTATUAL O MUNICIPAL
		if($this->muncod){
			$this->esfera = 'M'; // Municipal
		}else{
			$this->esfera = 'E'; // Estadual
		}
		
		// (EXCLUSIVO PARA O PAR) Carrega id do PAR caso seja processos de empenho e pagamento de subaçãoes, obras do PAR
		if($this->tipid == self::SUBACOES_GENERICO_PAR || $this->tipid == self::SUBACOES_PAR_EMENDAS ){
			$this->inuid = $this->pegaUm("SELECT inuid FROM par.instrumentounidade WHERE muncod = '".$this->muncod."'");
	    	if(!$this->inuid){
	    		$this->inuid = $this->pegaUm("SELECT inuid FROM par.instrumentounidade WHERE estuf = '".$this->estuf."' AND muncod is null");
	    	}
		}
	}
	
	public function getTipoProcesso(){
		$sql = "select sisid, tpdid, tipdescricao from execucaofinanceira.tipoprocesso where tipid = {$this->tipid} and tipstatus = 'A'";
		$arrTipos = $this->pegaLinha($sql);
		return $arrTipos;
	}
	
	public function montaCabecalhoEmpenho(){
		
		$arrTipo = $this->getTipoProcesso();
		
		if( $this->esfera == 'M' ){
			$municipio = $this->pegaUm("select mundescricao from territorios.municipio where muncod = '{$this->muncod}'");
		} else {
			$municipio = $this->pegaUm("select estdescricao from territorios.estado where estuf = '{$this->estuf}'");
		}
		$html = '<table class="tabela" id="cabecalhoempenho" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<tbody>
					<tr>
						<td class="subtituloDireita" style="width: 25%">Município:</td>
						<td style="width: 25%">'.$municipio.'</td>
						<td class="subtituloDireita">Tipo de Processo:</td>
						<td>'.$arrTipo['tipdescricao'].'</td>
					</tr>
					<tr>
						<td class="subtituloDireita" style="width: 25%">UF:</td>
						<td style="width: 25%">'.$this->estuf.'</td>
						<td class="subtituloDireita">Processo:</td>
						<td>'.$this->formataProcesso($this->pronumeroprocesso).'</td>
					</tr>
				</tbody>
				</table>';
		
		echo $html;
	}
	
	public function formataProcesso( $processo ){
		$processo = substr($processo,0,5) . "." . substr($processo,5,6) . "/" . substr($processo,11,4) . "-" . substr($processo,15,2);
		return $processo;
	}
}
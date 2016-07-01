<?php
include_once APPRAIZ . "demandasfies/classes/html_table.class.php";

class ResponsavelArea extends Modelo
{

	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "demandasfies.responsavelarea";

	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("reaid");

	/**
	 * Atributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array('reaid' => null, 'usucpf' => null, 'esdid' => null, 'reaprazo' => null, 'dmdid' => null, 'usucpfinclusao' => null, 'readtinclusao' => null,
		'usucpfinativacao' => null, 'readtinativacao' => null, 'reastatus' => null, 'reajustificativa' => null,'readtsubsidio'=>null,);

	public static function getArrayPerfilResponsaveis()
	{
		return array(
			ESD_DEMANDA_EM_CADASTRAMENTO => array('pflcod' => PFL_PROCURADOR_FEDERAL, 'area' => 'Procuradoria Federal'),
			ESD_DEMANDA_NUCLEO_JURIDICO => array('pflcod' => PFL_NUCLEO_JURIDICO, 'area' => 'Núcleo Jurídico'),
			ESD_DEMANDA_ADVOGADO => array('pflcod' => PFL_ADVOGADO, 'area' => 'Advogado'),
			ESD_DEMANDA_DTI_MEC => array('pflcod' => PFL_DTI_MEC, 'area' => 'DTI/MEC'),
			ESD_DEMANDA_ANALISTA_DTI_MEC => array('pflcod' => PFL_ANALISTA_DTI_MEC, 'area' => 'Analista DTI/MEC'),
			ESD_DEMANDA_EXECUCAO_DTI_MEC => array('pflcod' => PFL_ANALISTA_DTI_MEC, 'area' => 'Execução DTI/MEC'),
			ESD_DEMANDA_4_NIVEL => array('pflcod' => PFL_4_NIVEL, 'area' => '4º Nível'),
			ESD_DEMANDA_GESTOR_FIES => array('pflcod' => PFL_GESTOR_FIES, 'area' => 'Gestor FIES'),);
	}

	public static function getAreaResponsavelByEstado($esdid)
	{
		$perfis = ResponsavelArea::getArrayPerfilResponsaveis();
		return $perfis[$esdid];
	}

	public static function podeEditarResponsavel($perfis, $esdid)
	{
		// Regras para vinculação de responsáveis
		if ((in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis)) ||
			(in_array(PFL_PROCURADOR_FEDERAL, $perfis) && in_array($esdid, array(ESD_DEMANDA_EM_CADASTRAMENTO))) ||
			(in_array(PFL_NUCLEO_JURIDICO, $perfis) && in_array($esdid, array(ESD_DEMANDA_NUCLEO_JURIDICO, ESD_DEMANDA_ADVOGADO))) ||
			(in_array(PFL_DISTRIBUIDOR, $perfis) && in_array($esdid, array(ESD_DEMANDA_NUCLEO_JURIDICO))) ||
			(in_array(PFL_DTI_MEC, $perfis) && in_array($esdid, array(ESD_DEMANDA_DTI_MEC, ESD_DEMANDA_ANALISTA_DTI_MEC, ESD_DEMANDA_EXECUCAO_DTI_MEC))) ||
			(in_array(PFL_ANALISTA_DTI_MEC, $perfis) && in_array($esdid, array(ESD_DEMANDA_4_NIVEL))) ||
			(in_array(PFL_4_NIVEL, $perfis) && in_array($esdid, array(ESD_DEMANDA_4_NIVEL))) ||
			(in_array(PFL_GESTOR_FIES, $perfis) && in_array($esdid, array(PFL_GESTOR_FIES)))
		) {
			return true;
		}
		return false;
	}

	public static function podeEditarData($perfis, $esdid)
	{
		// Regras para estipulação de prazo
		if ( (in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis)) ||
			(in_array(PFL_PROCURADOR_FEDERAL, $perfis) && in_array($esdid, array(ESD_DEMANDA_EM_CADASTRAMENTO, ESD_DEMANDA_NUCLEO_JURIDICO))) ||
			(in_array(PFL_DISTRIBUIDOR, $perfis) && in_array($esdid, array(ESD_DEMANDA_NUCLEO_JURIDICO))) ||
			(in_array(PFL_NUCLEO_JURIDICO, $perfis) && in_array($esdid, array(ESD_DEMANDA_ADVOGADO, ESD_DEMANDA_DTI_MEC, ESD_DEMANDA_4_NIVEL, ESD_DEMANDA_GESTOR_FIES))) ||
			(in_array(PFL_ADVOGADO, $perfis) && in_array($esdid, array(ESD_DEMANDA_DTI_MEC, ESD_DEMANDA_4_NIVEL, ESD_DEMANDA_GESTOR_FIES))) ||
			(in_array(PFL_DTI_MEC, $perfis) && in_array($esdid, array(ESD_DEMANDA_ANALISTA_DTI_MEC, ESD_DEMANDA_EXECUCAO_DTI_MEC, ESD_DEMANDA_4_NIVEL))) ||
			(in_array(PFL_ANALISTA_DTI_MEC, $perfis) && in_array($esdid, array(ESD_DEMANDA_4_NIVEL))) ||
			(in_array(PFL_4_NIVEL, $perfis) && in_array($esdid, array(ESD_DEMANDA_DTI_MEC))) ||
			(in_array(PFL_GESTOR_FIES, $perfis) && in_array($esdid, array()))
		) {
			return true;
		}
	}

	public function getCpfAdvogadoDemanda($dmdid){
		$dmdid = (int)$dmdid;
		$pflAdvogado = PFL_ADVOGADO;
		$sql = "SELECT u.usucpf
        FROM demandasfies.responsavelarea ra
			LEFT JOIN seguranca.usuario u on u.usucpf = ra.usucpf
			INNER JOIN seguranca.perfilusuario  pu on u.usucpf = pu.usucpf
			INNER JOIN seguranca.perfil p on p.pflcod = pu.pflcod
        WHERE dmdid = {$dmdid} AND reastatus = 'A' AND p.pflcod = {$pflAdvogado}";

		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		if(!empty($dados)){
			return $dados[0]['usucpf'];
		}
		return $dados;
	}

	public function antesSalvar(){

		if(!empty($this->dmdid) && !empty($this->esdid) ){
			$sql = "update demandasfies.responsavelarea set reastatus = 'I' where dmdid = {$this->dmdid} and esdid = {$this->esdid}";
			$this->executar($sql);
			return true;
		}
		return false;
	}

	public function getHtmlTabelaHistoricoData($dados){
		$resp = $this->getAreaResponsavelByEstado($dados[0]['esdid']);
		$area = $resp['area'];
		$titulo = "<h3>{$area}</h3><br>";

		$tabelaArvore = new HTML_Table('relaorio_pdf_demandas_fies', 'table table-condensed table-bordered  table-responsive');
		$tabelaArvore->addTSection('thead');
		$tabelaArvore->addRow();

		$camposDaTabela = array('reajustificativa'=> 'Justificativa', 'readtinativacao'=> ' Data da inativação', 'reaprazo'=>'Data da Área', 'reastatus'=>'Status');
		foreach ($camposDaTabela as $campo) {
			$tabelaArvore->addCell($campo, '', 'header', array('class' => 'text-center'));
		}

		foreach ($dados as $key => $dado) {
			$tabelaArvore->addTSection('tbody');
			$tabelaArvore->addRow();

			foreach ($camposDaTabela as $atributo => $campo) {
				$class = 'text-center';
				$valor = $dado[$atributo];
				if($atributo == 'reaprazo' && !empty($valor) ){
					$valor = date('d/m/Y', strtotime($valor));
				}
				if( $atributo == 'readtinativacao' && !empty($valor)){
					$valor = date('d/m/Y', strtotime($valor));
				}
				if($atributo == 'reastatus'){
					$valor = ($valor == 'A' ? 'Ativo': 'Desativado');
				}
				$tabelaArvore->addCell($valor, $class);
			}
		}
		return $titulo.$tabelaArvore->display();
	}
}
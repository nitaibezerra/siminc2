<?php
class Par3_Controller_EntidadeParControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	}

	public static function recuperaDescricaoEntidadePar($entidade, $itrid){
		if($itrid == 1){
			$oEstado = new Estado();
			return $oEstado->descricaoEstado($entidade);
		} else {
			$oMunicipio = new Municipio();
			return $oMunicipio->descricaoMunicipio($entidade);
		}
	}

	public function listaEstado($filtro = array(), $tipo = '')
	{
		$arWhere = $arInner = array();

		$oEstado = new Estado();
		$oEstado->montaTabelaEstados($arWhere, $arInner, $tipo);
	}

	public function listaMunicipio($filtro = array(), $tipo = '')
	{
		$arWhere = $arInner = array();
		$join_capitais = $join_grandescidades = $join_indigena = $join_quilombola = $join_cidadania = $join_ideb = "";

		if($_SESSION['par']['arMuncod']){
			array_push($arWhere, "  m.muncod in ('". implode("','", $_SESSION['par']['arMuncod'])."') ");
		}

		if($filtro['pesquisa']){
			extract($filtro);

			# Programa
			if( $municipio ){
				//array_push($arWhere, " m.mundescricao ilike '%".$municipio."%' ");
				if( is_numeric($municipio) ){
					array_push($arWhere, "  m.muncod = '$municipio'");
				} else {
					$municipioTemp = removeAcentos($municipio);
					array_push($arWhere, " public.removeacento(m.mundescricao) ilike public.removeacento('%$municipioTemp%') ");
				}
			}

			# Estado
			if( $estuf ){
				array_push($arWhere, " m.estuf = '".$estuf."' ");
			}

			#Grupos de Municípios
			if( $grupo[0] ){
				$grupos = implode( $grupo, "," );
				$arInner['join_grupo'] .= "inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = m.estuf and mtm.tpmid IN (".$grupos.") ";
			}

		}

		$oMunicipio = new Municipio();

		$oMunicipio->montaTabelaEstados($arInner,$arWhere,$exporta, $tipo);
	}

	/**
	 * Método que grava Instrumento Unidade tanto para Estado como para Município
	 * @param unknown_type $arDados
	 * @return unknown_type
	 */
	public function gravaInstrumentoUnidade($arDados)
	{
		extract($arDados);

		$itrid = ($entidadePar == 'estado') ? 1 : 2;

		$oInstrumentoUnidade = new InstrumentoUnidade();
		if($itrid == 1){
			$inuidTemp = $oInstrumentoUnidade->verificaInuidEstado($estuf);
		} elseif($itrid == 2) {
			$inuidTemp = $oInstrumentoUnidade->verificaInuidMunicipio($muncod);
		}

		if(!trim($inuidTemp) && empty($inuidTemp)){
			$oInstrumentoUnidade = new InstrumentoUnidade();
			$oInstrumentoUnidade->inuid = null;
			$oInstrumentoUnidade->itrid = $itrid;
			$oInstrumentoUnidade->inudata = date('Y-m-d'); //$obData->formataData($tardatainicio,"YYYY-mm-dd");
			if($entidadePar == 'municipio'){
				$oInstrumentoUnidade->mun_estuf = $estuf;
				$oInstrumentoUnidade->muncod = $muncod;
			} else {
				$oInstrumentoUnidade->estuf = $estuf;
			}
			$oInstrumentoUnidade->usucpf = $_SESSION['usucpf'];
			$inuid = $oInstrumentoUnidade->salvar();
			$oInstrumentoUnidade->commit();
			unset($oInstrumentoUnidade);

			return $inuid;
		}

		return $inuidTemp;
	}

	public function visualizacaoDiagnostico($visao)
	{
		$oDimensao = new Dimensao();
		if($visao == 'arvore'){
			$this->visao->arDados = $oDimensao->lista('array', $_SESSION['par']['itrid']);
		}
		$this->visao->visualizar($visao);
	}

	//public static function lista($nrPaginaAtual = 1, $nrRegPorPagina = 5, $nrBlocoPaginacaoMaximo = 10, $nrBlocoAtual = 1)
	public static function lista($arConfiguracao = array(), $arWhere = array())
	{
		if(!empty($arConfiguracao) && is_array($arConfiguracao) ){
			extract($arConfiguracao);
		}

		$oDimensao = new Dimensao();
		//$sql = $oDimensao->lista('string', $_SESSION['par']['itrid'], $arWhere );
		$sql = $oDimensao->lista('string', $_SESSION['par']['itrid']);

		// CABEÇALHO da lista
		$arCabecalho = array("Comando","Descrição da Subação");

		// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho
		$arParamCol[0] = array("type" => Lista::TYPESTRING,
							   "style" => "width:1200px;",
							   "html" => "{dimcod}.{arecod}.{indcod} - {sbadsc}",
							   "align" => "left");
		/*$arParamCol[1] = array("type" => Lista::TYPESTRING,
							   //"style" => "color:red;",
							   "html" => "<img src=\"/imagens/fluxodoc.gif\" border=0 title=\"\" style=\"cursor:pointer;\" onclick=\"\">",
							   "align" => "center");
		*/
		$acao = "<center>
				   <!-- input onclick=\"\" {checked} type=\"checkbox\" name=\"rdo_grupo\" id=\"rdo_grupo_{gpdid}\" value=\"{gpdid}\"  -->
				   <img border=\"0\" src=\"../imagens/alterar.gif\" onclick=\"popupSubacao({sbaid})\" >
				 <center>";

		// ARRAY de parametros de configuração da tabela
		$arConfig = array(//"style" => "width:95%;",
						  "totalLinha" => false,
						  "totalRegistro" => true);

		$oPaginacaoAjax = new PaginacaoAjax();
		$oPaginacaoAjax->setNrPaginaAtual($nrPaginaAtual);
		$oPaginacaoAjax->setNrRegPorPagina($nrRegPorPagina);
		$oPaginacaoAjax->setNrBlocoPaginacaoMaximo($nrBlocoPaginacaoMaximo);
		$oPaginacaoAjax->setNrBlocoAtual($nrBlocoAtual);
		$oPaginacaoAjax->setDiv( 'divLista' );
		$oPaginacaoAjax->setCabecalho( $arCabecalho );
		$oPaginacaoAjax->setSql( $sql );
		$oPaginacaoAjax->setAcao( $acao );
		$oPaginacaoAjax->setParamCol( $arParamCol );
		$oPaginacaoAjax->setConfig( $arConfig );
		$oPaginacaoAjax->show();
	}

        public function carregarDadosMunicipioEstado($esfera, $cod, $arrDutid, $coluna){
            $obEntidadePar = new EntidadePar();
            return $obEntidadePar->carregarDadosMunicipioEstado($esfera, $cod, $arrDutid, $coluna);
        }
}
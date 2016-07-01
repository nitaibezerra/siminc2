<?php
class EntidadeParControle extends Controle
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
		if($_SESSION['par']['arEstuf']){
			array_push($arWhere, "  e.estuf in ('". implode("','", $_SESSION['par']['arEstuf'])."') ");
		}
		if($filtro['pesquisa']){
		extract($filtro);
		
			# começou diagnóstico
				if ( $comecoudiagnostico ) {
					$arInner['join_dadosunidade'] = "inner join par.dadosunidade dun on dun.inuid = iu.inuid ";
				}
			# questoes pontuais e dados unidade
				if($dadosunidadecheck && $questoespontuaischeck ){
					array_push($arWhere, " iu.inuid in ( select distinct inuid from par.parquestionario union all select distinct inuid from par.pontuacao WHERE ptostatus = 'A' ) ");
				}else if($questoespontuaischeck){
					array_push($arWhere, " iu.inuid in ( select distinct inuid from par.pontuacao WHERE ptostatus = 'A' ) ");
				}else if($dadosunidadecheck){
					array_push($arWhere, " iu.inuid in ( select distinct p.inuid from par.parquestionario p
															inner join questionario.resposta r ON r.qrpid = p.qrpid ) ");
				}
			# situação
				if ( $esdid ) {
					if( $esdid == 313 )
						array_push($arWhere, "( ed.esdid = {$_REQUEST["esdid"]} OR iu.docid is null  )");
					else
						array_push($arWhere, " ed.esdid = {$_REQUEST["esdid"]} ");
				}
			# não comecou diagnostico
				if( $naocomecoudiagnostico ){
					array_push($arWhere, " iu.inuid NOT IN ( SELECT dun.inuid FROM par.dadosunidade dun ) ");
				}
		}
		$oEstado = new Estado();
		$oEstado->lista($arWhere, $arInner, $tipo);
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
			
			# Tipo de obra
			if( $ptoid ){
				array_push($arWhere, " m.muncod in ( SELECT DISTINCT muncod FROM obras.preobra WHERE ptoid IN ('" . implode( "','", $ptoid ) . "') AND prestatus = 'A') ");
			}
			
			# Estado
			if( $estuf ){
				array_push($arWhere, " m.estuf = '".$estuf."' ");
			}
			
			#ProINFO
			if( $adesao_proinf ){
				array_push($arWhere, " m.muncod in ( SELECT DISTINCT muncod FROM obras.preobra  ) ");
			}
			
			# Analise
			//if( $analise ){
				//array_push($arWhere, " dc.muncod in ('" . implode( "','", $muncod ) . "') ");
			//}
			
			# capitais
			if ( $capitais ) {
				$arInner['join_capitais'] = "inner join territorios.estado e on e.estuf = m.estuf and e.muncodcapital = m.muncod";
			}
			
			#Grupos de Municípios
			if( $grupo[0] ){
				$grupos = implode( $grupo, "," );
				$arInner['join_grupo'] .= "inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = m.estuf and mtm.tpmid IN (".$grupos.") ";
			}
			
			if( $comsempar == 'sempar' ){
				array_push($arWhere, " ed.esdid in (314, 315) ");
			}
			if( $comsempar == 'compar' ){
				array_push($arWhere, " ed.esdid not in (314, 315) ");
			}
			
			# situação
			if ( $esdid ) {
				if( $esdid == WF_DIAGNOSTICO ){
					array_push($arWhere, " ( ( iu.docid is null ) OR ( ed.esdid = ".WF_DIAGNOSTICO." ) ) ");
				} elseif( $esdid == 99 ) {
					array_push($arWhere, " iu.inuid IN ( SELECT inuid FROM par.atualizacaopar ) ");
				} else {
					array_push($arWhere, " ed.esdid = {$_REQUEST["esdid"]} ");
				}
			}
			
			# começou diagnóstico
			if ( $comecoudiagnostico ) {
				array_push($arWhere, "   m.muncod  IN ( SELECT distinct m.muncod FROM par.dadosunidade du 
									INNER JOIN par.instrumentounidade iu on iu.inuid = du.inuid
									INNER JOIN territorios.municipio m ON iu.mun_estuf = m.estuf AND iu.muncod = m.muncod AND iu.itrid = 2
								)    ");
			}
		
			# Ideb
			if( is_array($ideb) && $ideb[0] ){
				$arInner['join_ideb'] = "inner join territorios.muntipomunicipio mti2 on mti2.muncod = m.muncod and mti2.estuf = m.estuf and mti2.tpmid in ('" . implode( "','", $ideb ) . "') ";
			}

			
			# questoes pontuais e dados unidade
				if($dadosunidadecheck && $questoespontuaischeck ){
					array_push($arWhere, " iu.inuid in ( select distinct inuid from par.parquestionario union all select distinct inuid from par.pontuacao WHERE ptostatus = 'A' ) ");
				}else if($questoespontuaischeck){
					array_push($arWhere, " iu.inuid in ( select distinct inuid from par.pontuacao WHERE ptostatus = 'A' ) ");
				}else if($dadosunidadecheck){
					array_push($arWhere, " iu.inuid in ( select distinct p.inuid from par.parquestionario p
															inner join questionario.resposta r ON r.qrpid = p.qrpid ) ");
				}
				if($naocomecoudiagnostico){
					array_push($arWhere, "   m.muncod NOT IN ( SELECT distinct m.muncod FROM par.dadosunidade du 
									INNER JOIN par.instrumentounidade iu on iu.inuid = du.inuid
									INNER JOIN territorios.municipio m ON iu.mun_estuf = m.estuf AND iu.muncod = m.muncod AND iu.itrid = 2
								)    ");
				}
		}
		$arrCPF = array('', '', '');
		
		if( !in_array($_SESSION['usucpf'], $arrCPF)  ) array_push($arWhere, "   m.estuf NOT IN ( 'DF' )");
		
		$oMunicipio = new Municipio();

		if($exporta == "true"){
			return $oMunicipio->lista($arInner,$arWhere,$exporta, $tipo);
		}else{
			$oMunicipio->lista($arInner,$arWhere,$exporta, $tipo);
		}
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
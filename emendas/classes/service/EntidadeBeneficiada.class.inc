<?php
class EntidadeBeneficiada extends cls_banco{
	
	public function __construct(){
		parent::__construct();
	}
	
	public function __destruct(){
		
		if( isset($_SESSION['transacao'] ) ){
			pg_query( $this->link, 'rollback; ');
			unset( $_SESSION['transacao'] );
		}
		
	}
	
	public function listaEntidadeBeneficiada( $post = array() ){
		$filtro = array();
		
		$obPost = (Object) $post;
	
		if($obPost->enbcnpj){
			$filtro[] = " lower(eb.enbcnpj) = '".ajustarDadosEntidade($obPost->enbcnpj)."'";
		}
		if($obPost->enbnome){
			$filtro[] = " lower(eb.enbnome) ilike '%".$obPost->enbnome."%'";
		}
		
		if(possuiPermissao()) {
			$acao = "( '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"Entidade('|| eb.enbid || ')\" \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
			   		   '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('|| eb.enbid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"> ' || 
			   		   '<img src=\"/imagens/refresh2.gif\" style=\"cursor: pointer\" onclick=\"consultarSituacaoEntidade('|| eb.enbcnpj||', '|| eb.enbid ||');\" border=0 alt=\"Ir\" title=\"Consultar Situacao Habilita\"></center>')";
		} else {
			$acao = "( '<center><img src=\"/imagens/alterar_01.gif\" style=\"cursor: pointer\" onclick=\"\" border=\"0\" alt=\"Ir\" title=\"Alterar\"> </a>' ||
			   		   '<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\" onclick=\"\" border=\"0\" alt=\"Ir\" title=\"Excluir\">' ||
			   		   '<img src=\"/imagens/refresh2_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Consultar Situacao Habilita\"></center>')";
		}
		
		/*if ($obPost->ordemlistadir <> 'DESC') {
			$ordemlistadir = 'ASC';$ordemlistadir2 = 'DESC';
		}else{
			$ordemlistadir = 'DESC'; $ordemlistadir2 = 'ASC';
		}*/
		
		$sql = $this->montaSQLLista( $acao, $filtro );
		
		//ver( simec_htmlentities( $sql ),d );
		/*if( $obPost->ordemlista <>'' ){
			$subsql = substr($sql,0,strpos(trim(strtoupper($sql)),'ORDER '));
			$sql = (!$subsql ? $sql : $subsql).' order by '.$obPost->ordemlista.' '.$ordemlistadir;
		}*/
		
		if(possuiPermissao()) {
			$adicEntidade = '<img src="/imagens/gif_inclui.gif" id="add" border="0" alt="Ir" title="Adicionar Entidade" onclick="Entidade(\'\');"><span style="cursor: pointer" onclick="Entidade(\'\');"> Adicionar Entidade Beneficiada</span>';
			$adicConsultaHabilita = '<img src="/imagens/refresh2.gif" id="add" border="0" alt="Ir" onclick="consultarSituacaoEntidade(\'\',\'\');"><span style="cursor: pointer" onclick="consultarSituacaoEntidade(\'\',\'\');"> Consultar Situacao Habilita</span>';
			$adicEnviarEmailEntidade = '<img src="/imagens/email.gif" id="add" border="0" alt="Ir" onclick="enviarEmailEntidade();"><span style="cursor: pointer" onclick="enviarEmailEntidade();"> Enviar Email Para <u>todas</u> Entidades da lista</span>';
		} else {
			$adicEntidade = '<img src="/imagens/gif_inclui_d.gif" id="add" border="0" alt="Ir" title="Adicionar Entidade" onclick=""><span style="cursor: pointer" onclick=""> Adicionar Entidade Beneficiada</span>';
			$adicConsultaHabilita = '<img src="/imagens/refresh2_01.gif" id="add" border="0" alt="Ir" ><span style="cursor: pointer"> Consultar Situacao Habilita</span>';
			$adicEnviarEmailEntidade = '<img src="/imagens/email.gif" id="add" border="0" alt="Ir" ><span style="cursor: pointer"> Enviar Email Entidade</span>';
		}
		
		$adciona = '<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="150%">';
		$adciona.= '<tr>';
		$adciona.= '	<td style="text-align: left;">';
		$adciona.= '		'.$adicConsultaHabilita.'';
		$adciona.= '	</td>';
		$adciona.= '	<td style="text-align: left;">';
		$adciona.= '		'.$adicEnviarEmailEntidade.'';
		$adciona.= '	</td>';
		$adciona.= '	<td style="text-align: right;">';
		$adciona.= '		'.$adicEntidade.'';
		$adciona.= '	</td>';
		$adciona.= '</tr>';
		$adciona.= '</table>';
		
		echo $adciona;
		
		monta_titulo( '', 'Lista de Entidade(s) Beneficiada(s)' );
		
		$arDados = parent::carregar( $sql );
		
		if( $arDados ){
			$ano = getdate();
			$registro = array();
			$arDadosArray = array();
			foreach ($arDados as $key => $value) {
				$key % 2 ? $cor = "" : $cor = "#f7f7f7";
				
				$sql = "SELECT 
							ur.usucpf, usunome 
						FROM 
							emenda.usuarioresponsabilidade ur
							inner join seguranca.usuario u on u.usucpf=ur.usucpf
						WHERE 
							ur.rpustatus='A' and ur.enbid = ".$value['enbid'];
				
				$arUserAut = parent::carregar($sql);
				
				$arUserAut = ($arUserAut ? $arUserAut : array());
				
				$userAut = "";
				if( $arUserAut ){
					foreach ($arUserAut as $usuario) {
						if( $userAut == "" ){
							$userAut = $usuario['usucpf'].' - '.$usuario['usunome'];
						} else {
							$userAut = $userAut.'<br> '.$usuario['usucpf'].' - '.$usuario['usunome'];
						}	
					}
				}
				
				$registro = array( "acao" => $value['acao'],
							   "enbcnpj" => $value['enbcnpj'],
							   "enbnome" => $value['enbnome'],			
							   "estuf" => $value['estuf'],			
							   "usucpf" => $userAut,
							   "enbsituacaohabilita" => $value['enbsituacaohabilita'],
							   "enbdataalteracao" => $value['enbdataalteracao']				
							  );
								  
				array_push($arDadosArray, $registro);		
			}
			
		}
		//if( $ano['year'] == $_SESSION['exercicio'] ){
			$cabecalho = array("Opção", "CNPJ", "Nome", "UF", "CPFs autorizados", "Situação Habilita", "Data Atualização");
		//} else {
		//	$cabecalho = array("Opção", "CNPJ", "Nome", "UF", "CPFs autorizados");
		//}
		parent::monta_lista_array($arDadosArray, $cabecalho, 20, 4, 'N','Center');
		echo $adciona;
	}
	
	public function montaSQLLista( $acao = null, $filtro = array() ){
		$acao = ( $acao ? $acao : "''" );
		$sql = "SELECT DISTINCT
				  ".$acao." as acao,
				  eb.enbid,
				  eb.enbcnpj,
				  eb.enbnome,
				  eb.estuf,
				  eb.enbsituacaohabilita,
				  to_char(eb.enbdataalteracao, 'DD/MM/YYYY HH24:MI:SS') as enbdataalteracao
				FROM 
					emenda.entidadebeneficiada eb 
				WHERE 
					eb.enbstatus = 'A'
					AND eb.enbano = {$_SESSION['exercicio']}
					". ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' )."
				order by eb.enbid";
		
		return $sql;
	}
	
	public function excluirEntidadeBeneficiadaAjax($enbid){		
		$sql = "UPDATE 
				  emenda.entidadebeneficiada  
				SET 
				  enbstatus = 'I'
				 
				WHERE 
				  enbid = $enbid";
		
		parent::executar($sql);
		echo parent::commit();
	}
	
	public function consultarSituacaoEntidade( $enbcnpj, $enbid ){
		$obHabilita = new Habilita();
		
		if( empty($enbcnpj) ){

			$arDados = parent::carregarObjeto( $this->montaSQLLista() );

			foreach ($arDados as $v) {
				$habilita = $obHabilita->consultaHabilitaEntidade($v->enbcnpj);
				if( $habilita != 1 ){
					$sql = "UPDATE emenda.entidadebeneficiada SET enbsituacaohabilita = '$habilita', enbdataalteracao = now()
							WHERE enbid = ".$v->enbid;
					parent::executar( iconv( "UTF-8", "ISO-8859-1", $sql) );
				}
			}
			if(parent::commit()){
				echo 'Operação realizada com sucesso!';			
			} else {
				echo 'Falha na operação!';
			}
		} else {
			$habilita = $obHabilita->consultaHabilitaEntidade( $enbcnpj );
			
			if( $habilita != 1 ){
				$situacao = $habilita;
			} else {			
				$situacao = utf8_encode('Entidade não habilitada'); 
			}
			$situacao = iconv( "UTF-8", "ISO-8859-1", $situacao);
			$sql = "UPDATE emenda.entidadebeneficiada SET enbsituacaohabilita = '$situacao', enbdataalteracao = now()
					WHERE enbid = $enbid";
			
			parent::executar( $sql );
			parent::commit();
		}
	}
	
	public function enviaEmailEntidade( $enbid = null ){
		if( !empty( $enbid ) ){
			$filtro = 'eb.enbid = '.$enbid;
		}
		$sql = "SELECT DISTINCT
					usu.usuemail
				FROM
					emenda.entidadebeneficiada eb
				    inner join emenda.usuarioresponsabilidade ure
				    	on ure.enbid = eb.enbid
				    inner join seguranca.usuario usu
				    	on usu.usucpf = ure.usucpf
				WHERE
					eb.enbstatus = 'A'
				    and ure.rpustatus = 'A'
				    and eb.enbano = '".$_SESSION['exercicio']."'
				    $filtro";
					
		return parent::carregarColuna($sql);
		/*$strAssunto = SIGLA_SISTEMA. " - Emenda";
		$strMensagem = '';
		
		return enviaEmailAnalise($strAssunto, $strMensagem, $strEmailTo);*/
	}
}
?>
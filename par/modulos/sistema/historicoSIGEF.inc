<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

$arrCPF = array('', '', '', '', '');

if( in_array($_SESSION['usucpf'], $arrCPF) ){
	$wsusuario 			= 'USAP_WS_SIGARP';
	$wssenha			= '03422625';
}
/*
define("USUARIO_SIGARP", 'USAP_WS_SIGARP');
# define("SENHA_SIGARP", '97635212');
// define("SENHA_SIGARP", '03422625'); // SEnha antiga modificada em 13/10 a pedido do Daniel e Murilo
define("SENHA_SIGARP", '03422625');*/

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="post" name="formulario" id="formulario" action="">
<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<tr>
		<td class="subtitulodireita">Histórico SIGEF:</td>
		<td>
			<input type="radio" name="tipo" id="tipo_e" value="E" <?php echo ( ($_REQUEST['tipo'] == 'E' || empty($_REQUEST['tipo']) ) ? 'checked="checked"' : '') ?> >Histórico de Empenho
			<input type="radio" name="tipo" id="tipo_p" value="P" <?php echo ($_REQUEST['tipo'] == 'P' ? 'checked="checked"' : '') ?> >Histórico de Pagamento
			<input type="radio" name="tipo" id="tipo_hp" value="HP" <?php echo ($_REQUEST['tipo'] == 'HP' ? 'checked="checked"' : '') ?> >Histórico Programa
		</td>
	</tr>
	<tr id="tr_processo">
		<td class="subtitulodireita">Nº Processo:</td>
		<td><?
			$numeroprocesso = $_REQUEST['numeroprocesso']; 
			echo campo_texto( 'numeroprocesso', 'N', 'S', '', 29, 200, '#####.######/####-##', ''); ?></td>
	</tr>
	<tr id="tr_programa">
		<td class="subtitulodireita">Programa FNDE:</td>
		<td><?
			$programa_fnde = $_REQUEST['programa_fnde']; 
			echo campo_texto( 'programa_fnde', 'N', 'S', '', 10, 2, '', ''); ?></td>
	</tr>
	<tr id="tr_exercicio">
		<td class="subtitulodireita">Ano Exercicio (opcional):</td>
		<td><?
			$anoexercicio = $_REQUEST['anoexercicio']; 
			echo campo_texto( 'anoexercicio', 'N', 'S', '', 10, 4, '[#]', ''); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Usuário:</b></td>
		<td>
			<input type="text" class="normal" name="usuario" id="usuario" value="<?php echo $wsusuario;?>" size="30" maxlength="15" title="Usuário">
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Senha:</b></td>
		<td>
			<input type="password" class="normal" name="senha" id="senha" value="<?php echo $wssenha;?>" size="30" maxlength="15" title="Senha">
		</td>
	</tr>
	<tr>
		<td class="subtituloDireita">&nbsp;</td>
		<td class="subtituloEsquerda"><input type="button" value="Pesquisar" id="btnPesquisar" /></td>
	</tr>
</table>
</form>
<script type="text/javascript">
$(document).ready(function(){
	
	if( $('[name="tipo"]:checked').val() == 'HP' ){
		$('#tr_processo').hide();
		$('#tr_programa').show();
		$('#tr_exercicio').show();
	} else {
		$('#tr_processo').show();
		$('#tr_programa').hide();
		$('#tr_exercicio').hide();
	}
});

$('[name="tipo"]').click(function(){
	if( $(this).val() == 'HP' ){
		$('#tr_processo').hide();
		$('#tr_programa').show();
		$('#tr_exercicio').show();
	} else {
		$('#tr_processo').show();
		$('#tr_programa').hide();
		$('#tr_exercicio').hide();
	}
});

$('#btnPesquisar').click(function(){
	var tipo = $('[name="tipo"]:checked').val();

	if( tipo == 'HP' ){
		if( jQuery('[name="programa_fnde"]').val() == '' ){
			alert('Informe o "Programa FNDE".');
			jQuery('[name="programa_fnde"]').focus();
			return false;
		}
	} else {
		if( jQuery('[name="numeroprocesso"]').val() == '' ){
			alert('Informe o "Nº Processo".');
			jQuery('[name="numeroprocesso"]').focus();
			return false;
		}
	}
	if( jQuery('[name="usuario"]').val() == '' ){
		alert('Informe o "Usuário".');
		jQuery('[name="usuario"]').focus();
		return false;
	}
	if( jQuery('[name="senha"]').val() == '' ){
		alert('Informe a "Senha".');
		jQuery('[name="senha"]').focus();
		return false;
	}
	jQuery('[name="requisicao"]').val('pesquisa');
	jQuery('#formulario').submit();
});
</script>
<?
if( $_REQUEST['requisicao'] == 'pesquisa' ){
	
	$_REQUEST['numeroprocesso'] = str_replace(".","", $_REQUEST['numeroprocesso']);
	$_REQUEST['numeroprocesso'] = str_replace("/","", $_REQUEST['numeroprocesso']);
	$_REQUEST['numeroprocesso'] = str_replace("-","", $_REQUEST['numeroprocesso']);
	$processo= $_REQUEST['numeroprocesso'];
	
	$wsusuario 	= $_REQUEST['usuario'];
	$wssenha 	= $_REQUEST['senha'];
	
	if( $_REQUEST['tipo'] == 'E' ){
		monta_titulo('Histórico de Empenho', '');
		echo historicoEmpenho($processo, $wsusuario, $wssenha);
	} elseif( $_REQUEST['tipo'] == 'HP' ) {
		monta_titulo('Histórico de Programa', '');
		$arrParam['co_programa'] 	= $_REQUEST['programa_fnde'];
		$arrParam['an_exercicio'] 	= $_REQUEST['anoexercicio'];
		$arrParam['wsusuario'] 		= $wsusuario;
		$arrParam['wssenha'] 		= $wssenha;
		echo historicoPrograma( $arrParam );
	} else {
		monta_titulo('Histórico de Pagamento', '');
		echo historicoPagamento($processo, $wsusuario, $wssenha);
	}
}

function historicoEmpenho($processo, $wsusuario, $wssenha){
	global $db;
	
	$data_created = date("c");
	
	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$wsusuario</usuario>
			<senha>$wssenha</senha>
		</auth>
		<params>
			<nu_processo>$processo</nu_processo>
		</params>
	</body>
</request>
XML;

	$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	//$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/pf';
	$xml = Fnde_Webservice_Client::CreateRequest()
		->setURL($urlWS)
		->setParams( array('xml' => $arqXml, 'method' => 'historicoempenho') )
		->execute();
	
	$xmlRetorno = $xml;
	$xml = simplexml_load_string( stripslashes($xml));
	
	if( (int)$xml->status->result == 1 && (int)$xml->status->message->code == 1 ){
		$executa = true;
		$arrXML = $xml->body->children();
		$arrXML2 = $xml->body->row->children();
		//ver($arrXML,d);
		$arrRegistro = array();
		foreach($arrXML as $chaves => $dados ){
			$cnpj 						= trim((string)$dados->cnpj);
			$programa_fnde 				= trim((string)$dados->programa_fnde);
			$unidade_gestora 			= trim((string)$dados->unidade_gestora);
			$numero_da_proposta_siconv 	= trim((string)$dados->numero_da_proposta_siconv);
			$numero_da_ne 				= trim((string)$dados->numero_da_ne);
			$numero_de_vinculacao_ne 	= trim((string)$dados->numero_de_vinculacao_ne);
			$valor_da_ne 				= trim((string)$dados->valor_da_ne);
			$numero_sequencial_da_ne 	= trim((string)$dados->numero_sequencial_da_ne);
			$nu_seq_mov_ne 				= trim((string)$dados->nu_seq_mov_ne);
			$data_do_empenho 			= trim((string)$dados->data_do_empenho);
			$cpf 						= trim((string)$dados->cpf);
			$nu_id_sistema 				= trim((string)$dados->nu_id_sistema);
			$descricao_do_empenho 		= utf8_decode( trim((string)$dados->descricao_do_empenho) );
			$ano_do_empenho 			= trim((string)$dados->ano_do_empenho);
			$centro_de_gestao 			= trim((string)$dados->centro_de_gestao);		
			$natureza_de_despesa 		= trim((string)$dados->natureza_de_despesa);
			$fonte_de_recurso 			= trim((string)$dados->fonte_de_recurso);
			$ptres 						= trim((string)$dados->ptres);
			$esfera 					= trim((string)$dados->esfera);
			$pi 						= trim((string)$dados->pi);
			$cod_especie 				= trim((string)$dados->cod_especie);
			$numero_do_processo 		= trim((string)$dados->numero_do_processo);
			$situacao_do_empenho 		= trim((string)$dados->situacao_do_empenho);
			
			array_push($arrRegistro, array(
										'cnpj' => $cnpj,
										'programa_fnde' => $programa_fnde,
										'unidade_gestora' => $unidade_gestora,
										'numero_da_proposta_siconv' => $numero_da_proposta_siconv,
										'numero_da_ne' => $numero_da_ne,
										'numero_de_vinculacao_ne' => $numero_de_vinculacao_ne,
										'valor_da_ne' => number_format($valor_da_ne, 2, ',', '.'),
										'numero_sequencial_da_ne' => $numero_sequencial_da_ne,
										'nu_seq_mov_ne' => $nu_seq_mov_ne,
										'data_do_empenho' => $data_do_empenho,
										'cpf' => $cpf,
										'nu_id_sistema' => $nu_id_sistema,
										'descricao_do_empenho' => $descricao_do_empenho,
										'ano_do_empenho' => $ano_do_empenho,
										'centro_de_gestao' => $centro_de_gestao,
										'natureza_de_despesa' => $natureza_de_despesa,
										'fonte_de_recurso' => $fonte_de_recurso,
										'ptres' => $ptres,
										'esfera' => $esfera,
										'pi' => $pi,
										'cod_especie' => $cod_especie,
										//'numero_do_processo' => $numero_do_processo,
										'situacao_do_empenho' => $situacao_do_empenho
									 )
						);
		}
		$cabecalho = array('cnpj', 'programa_fnde', 'unidade_gestora', 'numero_da_proposta_siconv', 'numero_da_ne', 'numero_de_vinculacao_ne', 'valor_da_ne', 'numero_sequencial_da_ne', 'nu_seq_mov_ne', 'data_do_empenho', 
				'cpf', 'nu_id_sistema', 'descricao_do_empenho', 'ano_do_empenho', 'centro_de_gestao', 'natureza_de_despesa', 'fonte_de_recurso', 'ptres', 'esfera', 'pi', 'cod_especie', 'situacao_do_empenho');
		array_multisort($arrRegistro, SORT_DESC);
		return $db->monta_lista_simples($arrRegistro, $cabecalho,1000000,5,'N','100%', 'S', '', '', '', true);
	} else {
		print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		print '<tr><td align="center" style="color:#cc0000;">'.($xml->status->message->code.' - '.utf8_decode($xml->status->message->text)).'<br>'.utf8_decode($xml->status->error->message->text).'</td></tr>';
		print '</table>';
	}
}

function historicoPagamento( $processo, $wsusuario, $wssenha ){
	global $db;
	$data_created = date("c");
	
$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$wsusuario</usuario>
			<senha>$wssenha</senha>
		</auth>
		<params>
			<nu_processo>$processo</nu_processo>
		</params>
	</body>
</request>
XML;

	//$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/pf';
	$xml = Fnde_Webservice_Client::CreateRequest()
		->setURL($urlWS)
		->setParams( array('xml' => $arqXml, 'method' => 'historicopagamento') )
		->execute();
	
	$xmlRetorno = $xml;
	$xml = simplexml_load_string( stripslashes($xml));
	//ver($xml,d);
	if( (int)$xml->status->result == 1 && (int)$xml->status->message->code == 1 ){
		$executa = true;
		$arrXML = $xml->body->children();
		$arrXML2 = $xml->body->row->children();
		//ver($arrXML,d);
		$arrRegistro = array();
		foreach($arrXML as $chaves => $dados ){
			$nu_parcela					= trim((string)$dados->nu_parcela);
			$an_exercicio 				= trim((string)$dados->an_exercicio);
			$vl_parcela 				= trim((string)$dados->vl_parcela);
			$nu_mes 					= trim((string)$dados->nu_mes);
			$nu_documento_siafi_ne 		= trim((string)$dados->nu_documento_siafi_ne);
			$nu_seq_mov_ne 				= trim((string)$dados->nu_seq_mov_ne);
			$ds_username_movimento 		= trim((string)$dados->ds_username_movimento);
			$ds_situacao_doc_siafi 		= trim((string)$dados->ds_situacao_doc_siafi);			
			$dt_movimento 				= trim((string)$dados->dt_movimento);			
			$nu_seq_mov_pag 			= trim((string)$dados->nu_seq_mov_pag);			
			$dt_emissao 				= trim((string)$dados->dt_emissao);
			$nu_documento_siafi 		= trim((string)$dados->nu_documento_siafi);
			$numero_de_vinculacao 		= trim((string)$dados->numero_de_vinculacao);
			
			array_push($arrRegistro, array(
										'nu_parcela' 			=> $nu_parcela,
										'an_exercicio' 			=> $an_exercicio,
										'vl_parcela' 			=> $vl_parcela,
										'nu_mes' 				=> $nu_mes,
										'nu_documento_siafi_ne' => $nu_documento_siafi_ne,
										'nu_seq_mov_ne' 		=> $nu_seq_mov_ne,
										'ds_username_movimento'	=> $ds_username_movimento,
										'ds_situacao_doc_siafi' => $ds_situacao_doc_siafi,
										'dt_movimento' 			=> $dt_movimento,
										'nu_seq_mov_pag' 		=> $nu_seq_mov_pag,
										'dt_emissao' 			=> $dt_emissao,
										'nu_documento_siafi' 	=> $nu_documento_siafi,
										'numero_de_vinculacao' 	=> $numero_de_vinculacao
									 )
						);
		}
		$cabecalho = array('nu_parcela', 'an_exercicio', 'vl_parcela', 'nu_mes', 'nu_documento_siafi_ne', 'nu_seq_mov_ne', 'ds_username_movimento', 'ds_situacao_doc_siafi', 
				'dt_movimento', 'nu_seq_mov_pag', 'dt_emissao', 'nu_documento_siafi', 'numero_de_vinculacao');
		array_multisort($arrRegistro, SORT_DESC);
		return $db->monta_lista_simples($arrRegistro, $cabecalho,1000000,5,'N','100%', 'S', '', '', '', true);
	} else {
		print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		print '<tr><td align="center" style="color:#cc0000;">'.($xml->status->message->code.' - '.utf8_decode($xml->status->message->text)).'<br>'.utf8_decode($xml->status->error->message->text).'</td></tr>';
		print '</table>';
	}
}

function historicoPrograma( $arrParam ){
	global $db;
	$data_created = date("c");
	
	$co_programa 	= $arrParam['co_programa'];
	$an_exercicio 	= $arrParam['an_exercicio'];
	$wsusuario 		= $arrParam['wsusuario'];
	$wssenha 		= $arrParam['wssenha'];
	
$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$wsusuario</usuario>
			<senha>$wssenha</senha>
		</auth>
		<params>
			<co_programa>$co_programa</co_programa>
        	<an_exercicio>$an_exercicio</an_exercicio>		
		</params>
	</body>
</request>
XML;

	//$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	$urlWS = 'hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/pf';
	$xml = Fnde_Webservice_Client::CreateRequest()
		->setURL($urlWS)
		->setParams( array('xml' => $arqXml, 'method' => 'historicoprograma') )
		->execute();
	
	//ver( simec_htmlentities($arqXml), simec_htmlentities($xml),d );
	
	$xmlRetorno = $xml;
	$xml = simplexml_load_string( stripslashes($xml));
	//ver($xml,d);
	if( (int)$xml->status->result == 1 && (int)$xml->status->message->code == 1 ){
		$executa = true;
		$arrXML = $xml->body->children();
		$arrXML2 = $xml->body->row->children();
		//ver($arrXML,d);
		$arrRegistro = array();
		foreach($arrXML as $chaves => $dados ){
			$nu_parcela					= trim((string)$dados->nu_parcela);
			$an_exercicio 				= trim((string)$dados->an_exercicio);
			$vl_parcela 				= trim((string)$dados->vl_parcela);
			$nu_mes 					= trim((string)$dados->nu_mes);
			$nu_documento_siafi_ne 		= trim((string)$dados->nu_documento_siafi_ne);
			$nu_seq_mov_ne 				= trim((string)$dados->nu_seq_mov_ne);
			$ds_username_movimento 		= trim((string)$dados->ds_username_movimento);
			$ds_situacao_doc_siafi 		= trim((string)$dados->ds_situacao_doc_siafi);			
			$dt_movimento 				= trim((string)$dados->dt_movimento);			
			$nu_seq_mov_pag 			= trim((string)$dados->nu_seq_mov_pag);			
			$dt_emissao 				= trim((string)$dados->dt_emissao);
			$nu_documento_siafi 		= trim((string)$dados->nu_documento_siafi);
			$numero_de_vinculacao 		= trim((string)$dados->numero_de_vinculacao);
			
			array_push($arrRegistro, array(
										'nu_parcela' 			=> $nu_parcela,
										'an_exercicio' 			=> $an_exercicio,
										'vl_parcela' 			=> $vl_parcela,
										'nu_mes' 				=> $nu_mes,
										'nu_documento_siafi_ne' => $nu_documento_siafi_ne,
										'nu_seq_mov_ne' 		=> $nu_seq_mov_ne,
										'ds_username_movimento'	=> $ds_username_movimento,
										'ds_situacao_doc_siafi' => $ds_situacao_doc_siafi,
										'dt_movimento' 			=> $dt_movimento,
										'nu_seq_mov_pag' 		=> $nu_seq_mov_pag,
										'dt_emissao' 			=> $dt_emissao,
										'nu_documento_siafi' 	=> $nu_documento_siafi,
										'numero_de_vinculacao' 	=> $numero_de_vinculacao
									 )
						);
		}
		$cabecalho = array('nu_parcela', 'an_exercicio', 'vl_parcela', 'nu_mes', 'nu_documento_siafi_ne', 'nu_seq_mov_ne', 'ds_username_movimento', 'ds_situacao_doc_siafi', 
				'dt_movimento', 'nu_seq_mov_pag', 'dt_emissao', 'nu_documento_siafi', 'numero_de_vinculacao');
		array_multisort($arrRegistro, SORT_DESC);
		return $db->monta_lista_simples($arrRegistro, $cabecalho,1000000,5,'N','100%', 'S', '', '', '', true);
	} else {
		print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		print '<tr><td align="center" style="color:#cc0000;">'.($xml->status->message->code.' - '.utf8_decode($xml->status->message->text)).'<br>'.utf8_decode($xml->status->error->message->text).'</td></tr>';
		print '</table>';
	}
}
?>
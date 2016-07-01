<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<?php
header('content-type: text/html; charset=ISO-8859-1');
$terid = $_REQUEST['terid'];

$sql = "SELECT
			to_char(terdataassinatura, 'DD/MM/YYYY') as terdataassinatura,
			usucpfassinatura,
			terassinado, proid
		FROM
			par.termocompromissopac 
		WHERE 
			terid = ".$terid;
$dados = $db->pegaLinha( $sql );

$terdocumento = pegaTermoCompromissoArquivo( '', $terid );

if( $terdocumento == '' ){
	$html = retornaHTMLTermo( $terid );
	
	gravaHtmlDocumento($html, $terid, $dados['proid'], 'PAC');
	
} else {
	$html = $terdocumento;
}


if( $_GET['aceita'] == 'S' ){
	ini_set("memory_limit","1000M");
	set_time_limit(0);
	
	$sql = "UPDATE par.termocompromissopac SET 
			  terdataassinatura = now(),
			  usucpfassinatura = '".$_SESSION['usucpf']."',
			  terassinado = 't'
			WHERE 
			  terid = ".$_REQUEST['terid'];
	
	$db->executar( $sql );
	if($db->commit()){
		include_once(APPRAIZ."par/classes/WSSigarp.class.inc");
		$oWSSigarp = new WSSigarp();
		
		$proid = $db->pegaUm("select 
								ppc.proid
							from 
								par.termocompromissopac tcp
							INNER JOIN par.processoobraspaccomposicao ppc ON ppc.proid = tcp.proid and ppc.pocstatus = 'A'
							INNER JOIN obras.preobra pre ON pre.preid = ppc.preid
							INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
							WHERE
								tcp.terstatus = 'A' AND pto.ptocategoria IS NOT NULL AND tcp.terid = {$_REQUEST['terid']}");
		if( $proid ) {
			$sigarp = $oWSSigarp->solicitarItemObra($_REQUEST['terid'], $proid);
			/*
			$arrObras = $db->carregar("SELECT 
													m.mundescricao || ' - ' || m.estuf as local,
													pre.predescricao as nome,
													pre.obrid as obrid												
												FROM 
													par.termoobraspaccomposicao tpc 
												INNER JOIN obras.preobra pre ON pre.preid = tpc.preid
												INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND ptocategoria IS NOT NULL
												INNER JOIN territorios.municipio m on m.muncod = pre.muncod
												WHERE 
													tpc.terid = ".$_REQUEST['terid']);
			
			if( is_array($arrObras) && $arrObras[0] ){ // É de MI, então envio o e-mail
				$envio = enviaEmailMI($_REQUEST['terid'], $arrObras);
			}
			*/
			
			$sql = "SELECT DISTINCT 
						po.preid
					FROM par.empenhoobra  p
						INNER JOIN obras.preobra po ON p.preid = po.preid  AND po.prestatus = 'A' and eobstatus = 'A'
						INNER JOIN par.empenho emp on emp.empid = p.empid and empstatus <> 'I'
						INNER JOIN par.processoobra pro on pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
					WHERE
						pro.proid = $proid";
			
			$arrObra = $db->carregarColuna($sql);
			$arrObra = $arrObra ? $arrObra : array();
			
			$preObra = new PreObra();
			foreach ($arrObra as $preid) {					
				$preObra->importarPreobraParaObras2( $preid );
			}
		}
		echo "<script>
					alert('Operação realizada com sucesso!');
					window.opener.location.href = 'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A';
					window.close();
				</script>";
		exit();
	} else{
		echo "<script>
					alert('Falha na operação!');
				</script>";
	}
}

$perfil = pegaArrayPerfil($_SESSION['usucpf']);

$display = 'style="display: none;"';
$displayValidado = 'style="display: none;"';

if( in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
	in_array(PAR_PERFIL_PREFEITO, $perfil) ||
	in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
	in_array(PAR_PERFIL_PREFEITO, $perfil) || 
	in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) 
){
	if( $dados['terassinado'] == 't' ){
		$sql = "SELECT usunome FROM seguranca.usuario WHERE usucpf = '".$dados['usucpfassinatura']."'";
		$nome = $db->pegaUm( $sql );
		$display = 'style="display: none;"';
		$displayValidado = '';
	} else {
		$display = '';
	}
}

if( $_SESSION['par']['muncod'] ){
	if( $nome == '' ){
		$nome = 'manualmente';
	} else {
		$nome = 'pelo(a) Prefeito(a) '.$nome.' - CPF: '.formatar_cpf($dados['usucpfassinatura']).' em '.$dados['terdataassinatura'];
	}
} else {
	if( $nome == '' ){
		$nome = 'manualmente';
	} else {
		$nome = 'Secretário(a) de Educação '.$nome.' - CPF: '.formatar_cpf($dados['usucpfassinatura']).' em '.$dados['terdataassinatura'];
	}
}
//if( $html['tpdcod'] != '102' ) $display = 'style="display: none;"';

function monta_cabecalho_relatorio_par( $data = '', $largura ){
	
	global $db;
	
	$data = $data ? $data : date( 'd/m/Y' );
	
	$cabecalho = '<table width="'.$largura.'%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">'
				.'	<tr bgcolor="#ffffff">' 	
				.'		<td valign="top" align="center"><img src="../imagens/brasao.gif" width="45" height="45" border="0">'			
				//.'		<td nowrap align="center" valign="middle" height="1" style="padding:5px 0 0 0;">'				
				.'			<br><b>MINISTÉRIO DA EDUCAÇÃO<br/>'				
//				.'			Acompanhamento da Execução Orçamentária<br/>'					
				.'			FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />'
				.'		</td>'
				//.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'					
				//.'			Impresso por: <b>' . $_SESSION['usunome'] . '</b><br/>'					
				//.'			Hora da Impressão: '.$data .' - ' . date( 'H:i:s' ) . '<br />'					
				//.'		</td>'	
				.'	</tr>'					
				.'</table><br><br>';					
								
		return $cabecalho;						
						
}
?>
<style>

@media print {.notprint { display: none } .div_rolagem{display: none} }	
@media screen {.notscreen { display: none; }
.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
 
</style>
<script type="text/javascript">
	function aceitarTermo(terid){
		if(confirm('Eu li, e estou de acordo com o termo de compromisso.')){
			document.getElementById('aceitar').disabled = true;
			window.location.href = 'par.php?modulo=principal/teladevalidacao&acao=A&terid='+terid+'&aceita=S';
		}
	}
</script>
<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubtituloDireita, div_rolagem" style="text-align: center;">
			<input type="button" name="aceitar" <?=$display; ?>  id="aceitar" value="Aceitar" onclick="aceitarTermo(<?=$terid ?>)" />
			<input type="button" name="impressao" id="impressao" value="Impressão" onclick="window.print();" />
			<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
		</td>
	</tr>
</table>
<table id="termo" width="95%" align="left" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td style="font-size: 12px; font-family:arial;">
			<div>
			<?php 
				echo html_entity_decode ($html, ENT_QUOTES, 'ISO-8859-1');
			?>
			</div>
		</td>
	</tr>
</table>
<br>
<br>
<br>
<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr <?=$displayValidado; ?> style="text-align: center;">
		<td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>
			<b>Validado <?=$nome ?></b>
		</td>
	</tr>
</table>
<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubtituloDireita, div_rolagem" style="text-align: center;">
			<input type="button" name="aceitar" <?=$display; ?>  id="aceitar" value="Aceitar" onclick="aceitarTermo(<?=$terid ?>)" />
			<input type="button" name="impressao" id="impressao" value="Impressão" onclick="window.print();" />
			<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
		</td>
	</tr>
</table>
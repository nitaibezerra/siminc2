<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<?php
$sql = "select * from (
			SELECT dpv.dpvcpf as cpfgestor, 
						to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data, d.mdoid,
						us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, 
						( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado
					FROM par.documentopar  dp
					INNER JOIN par.modelosdocumentos   		d   ON d.mdoid   = dp.mdoid
					INNER JOIN par.processopar 				pp  ON pp.prpid  = dp.prpid and pp.prpstatus = 'A'
					INNER JOIN par.instrumentounidade 		iu  ON iu.inuid  = pp.inuid
					LEFT  JOIN par.documentoparvalidacao 	dpv ON dpv.dopid = dp.dopid AND dpv.dpvstatus = 'A'
					LEFT  JOIN seguranca.usuario 			us  ON us.usucpf = dpv.dpvcpf
			UNION 
			SELECT dpv.dpvcpf as cpfgestor, 
						to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data, d.mdoid, 
						us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, 
						( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado
					FROM par.documentopar  dp
					INNER JOIN par.modelosdocumentos   		d   ON d.mdoid   = dp.mdoid
					INNER JOIN par.processoobraspar 		pp  ON pp.proid  = dp.proid and pp.prostatus = 'A'
					INNER JOIN par.instrumentounidade 		iu  ON iu.inuid  = pp.inuid
					LEFT  JOIN par.documentoparvalidacao 	dpv ON dpv.dopid = dp.dopid AND dpv.dpvstatus = 'A'
					LEFT  JOIN seguranca.usuario 			us  ON us.usucpf = dpv.dpvcpf
			) as foo
		 WHERE dopid = ".$_REQUEST['dopid']; 

$html = $db->pegaLinha($sql);

$doptexto = pegaTermoCompromissoArquivo( $_REQUEST['dopid'], '' );

$sql = "SELECT 
			dpv.dpvcpf as cpfgestor, 
			to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data, 
			us.usunome, 
			us.usucpf
		FROM par.documentopar  dp
		INNER JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid
		INNER JOIN seguranca.usuario us ON us.usucpf = dpv.dpvcpf
		WHERE 
			dpv.dpvstatus = 'A' and
			dp.dopid = ".$_REQUEST['dopid'];

$dadosValidacao = $db->carregar($sql);

$textoValidacao = "";

if(is_array($dadosValidacao)){
	foreach( $dadosValidacao as $dv ){
		$cpfvalidacao[] = $dv['cpfgestor'];
		$textoValidacao .= "<b>Validado por ".$dv['usunome']." - CPF: ".formatar_cpf($dv['usucpf'])." em ".$dv['data']." </b><br>";
	}
}

$cpfvalidacao = $cpfvalidacao ? $cpfvalidacao : array();

$perfil = pegaArrayPerfil($_SESSION['usucpf']);

//if( !empty($html['cpfgestor']) ){
if( $html['mdoqtdvalidacao'] == $html['qtdvalidado'] || in_array($_SESSION['usucpf'], $cpfvalidacao) ){
	$display = 'style="display: none;"';
	$displayValidado = '';
} else {
        #Nao existia validacao de perfil nessa tela foi adicionada a mesma validacao utilizada na teladeassinatura.inc
	if( in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
//        in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || # Removido devido solicitacao dos administradores
		in_array(PAR_PERFIL_PREFEITO, $perfil) || 
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) || 
		( in_array(PAR_PERFIL_UNIVERSIDADE_ESTADUAL, $perfil) && in_array( $html['mdoid'], Array(34, 40) ) )  ||
		( in_array(PAR_PERFIL_ENTIDADE_EXECUTORA, $perfil) )
	){
		$display = '';
	} else {
		$display = 'style="display: none;"';
	}
        # Fim validacao perfil

	$displayValidado = 'style="display: none;"';
	if( $html['qtdvalidado'] >= 1 ){
		$displayValidado = '';
	}
}

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
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function aceitarTermo(dopid){
		if(confirm('Eu li, e estou de acordo com o termo de compromisso.')){
			$('#aceitar').attr('disabled',true);
			$('#requisicao').val('aceita');
			
			$('#formulario').submit();
			
			<?php /*if( $_REQUEST['validacao'] == 'S' ){ ?>
				window.location.href = 'par.php?modulo=principal/teladeassinatura&acao=A&validacao=S&dopid='+dopid+'&aceita=S';
			<?php } else { ?>
				window.location.href = 'par.php?modulo=principal/teladeassinatura&acao=A&dopid='+dopid+'&aceita=S';
			<?php }*/ ?>
		}
	}
</script>
<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="dopid" id="dopid" value="<?=$_REQUEST['dopid'] ?>">
	<input type="hidden" name="validacao" id="validacao" value="<?=$_REQUEST['validacao']; ?>">
	
	<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubtituloDireita, div_rolagem" style="text-align: center;">
                            <?php if(empty($display)): ?>
				<input type="button" name="aceitar" <?=$display; ?>  id="aceitar" value="Aceitar" onclick="aceitarTermo(<?=$_REQUEST['dopid'] ?>)" />
                            <?php endif; ?>
				<input type="button" name="impressao" id="impressao" value="Impressão" onclick="window.print();" />
				<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
			</td>
		</tr>
	</table>
		<?
		$cabecalhoBrasao .= "<table width=\"95%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" >";
	$cabecalhoBrasao .= "<tr>" .
					"<td colspan=\"100\">" .($html['tpdcod'] == '101' ? monta_cabecalho_relatorio_par('29/11/2011', '100') : monta_cabecalho_relatorio_par('', '100') ).
					"</td>" .
				  "</tr>
				  </table>";
						echo $cabecalhoBrasao;
		?>
	<table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
		<tr>
			<td style="font-size: 12px; font-family:arial;">
				<div>
				<?php
					echo html_entity_decode ($doptexto);
				?>
				</div>
			</td>
		</tr>
	</table>
	<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
		<tr <?=$displayValidado; ?> style="text-align: center;">
			<td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>
				<?=$textoValidacao; ?>
			</td>
		</tr>
	</table>
	<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubtituloDireita, div_rolagem" style="text-align: center;">
				<input type="button" name="impressao" id="impressao" value="Impressão" onclick="window.print();" />
				<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
			</td>
		</tr>
	</table>
</form>
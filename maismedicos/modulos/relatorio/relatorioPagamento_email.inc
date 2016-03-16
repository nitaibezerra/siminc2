<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script>
function confirmaEnvioEmail()
{
	if(confirm('Deseja enviar os e-mails?')){
		//document.location.href = '/maismedicos/maismedicos.php?modulo=relatorio/relatorioPagamento&acao=A&requisicao=email&confirmaEnvio=true';
		document.formulario.submit();
	}
}
</script>
<?php //ver($_REQUEST); ?>
<form id="formulario" name="formulario" method="post">
	<input type="hidden" name="requisicao" value="email" />
	<input type="hidden" name="confirmaEnvio" value="true" />
	<input type="hidden" name="tutcpf" value="<?php echo $_POST['tutcpf']; ?>" />
	<input type="hidden" name="tutnome" value="<?php echo $_POST['tutnome']; ?>" />
	<?php if($_REQUEST['strid']):?>
		<?php foreach($_REQUEST['strid'] as $strid_): ?>
			<input type="hidden" name="strid[]" value="<?php echo $strid_; ?>" />
		<?php endforeach;?>
	<?php endif; ?>
	<?php if($_REQUEST['uniid']):?>
		<?php foreach($_REQUEST['uniid'] as $uniid_): ?>
			<input type="hidden" name="uniid[]" value="<?php echo $uniid_; ?>" />
		<?php endforeach;?>
	<?php endif; ?>
	<input type="hidden" name="tuttipo" value="<?php echo $_POST['tuttipo']; ?>" />	
	<input type="hidden" name="mesperiodo" value="<?php echo $_POST['mesperiodo']; ?>" />
	<input type="hidden" name="anoperiodo" value="<?php echo $_POST['anoperiodo']; ?>" />	
</form>
<?php 
$conteudo = '';
$email_enviado = 0;
$boExisteEnvio = false;
?>
<?php if(!empty($_REQUEST['uniid'][0])): ?>
	<?php foreach($_REQUEST['uniid'] as $uniid): ?>
		
		<?php 
		unset($_REQUEST['uniid']);
		extract($_REQUEST);
		
		$sqlTutores = "select distinct
							
							tut.tutnome,
							tut.tutemail
							
						from
							maismedicos.tutor tut
							
						inner join
							maismedicos.universidade uni ON uni.uniid = tut.uniid
						inner join
							maismedicos.remessadetalhe det ON det.tutid = tut.tutid
						inner join
							maismedicos.remessacabecalho cab ON cab.rmcid = det.rmcid
						inner join
							maismedicos.folhapagamento fpg ON fpg.fpgid = cab.fpgid
						inner join
							maismedicos.autorizacaopagamento apg on apg.rmdid = det.rmdid and apg.apgstatus = 'A'
					
						where
							tut.tutstatus = 'A'
						and
							tut.tuttipo = 'T'
						and 
							tut.uniid = {$uniid}
						order by
							tut.tutnome";
		
		$rsTutores = $db->carregar($sqlTutores);	

		if(!empty($_POST['tutcpf'])){
			$_POST['tutcpf'] = str_replace(array('.','-'),'', $_POST['tutcpf']);
			$arWhere[] = "tut.tutcpf = '{$_POST['tutcpf']}'";
		}
		if(!empty($_POST['tutnome'])){
			$arWhere[] = "tut.tutnome ilike '%{$_POST['tutnome']}%'";
		}
		if( $strid[0] && $strid_campo_flag ){
			$arWhere[] = " sit.strid ". (!$strid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( ".implode( ",", $strid )." ) ";
		}		
		if(!empty($_POST['tuttipo']) && $_POST['tuttipo'] != 'A'){
			$operador = $_POST['tuttipo'] == 'T' ? '=' : '<>';
			$arWhere[] = "tut.tuttipo {$operador} 'T'";
		}
		if(!empty($_POST['mesperiodo']) && !empty($_POST['anoperiodo'])){
			$_POST['mesperiodo'] = str_pad($_POST['mesperiodo'], 2, "0", STR_PAD_LEFT);
			$arWhere[] = "apg.apgmes = '{$_POST['mesperiodo']}' and apg.apgano = '{$_POST['anoperiodo']}'";
		}
		
		$sql = "select
					tut.tutnome,
					case when tut.tuttipo = 'T'
					then 'Tutor'
					else 'Supervisor'
					end as funcao,
					case when tut.tuttipo = 'T'
					then '5000'
					else '4000'
					end as valor,
					fpgdsc,
					uni.uninome,
					coalesce(sit.strdsc,'N/A') as tpodsc
				from
					maismedicos.tutor tut
				inner join
					maismedicos.universidade uni ON uni.uniid = tut.uniid
				inner join
					maismedicos.remessadetalhe det ON det.tutid = tut.tutid
				inner join
					maismedicos.remessacabecalho cab ON cab.rmcid = det.rmcid
				inner join
					maismedicos.folhapagamento fpg ON fpg.fpgid = cab.fpgid
				inner join
					maismedicos.autorizacaopagamento apg on apg.rmdid = det.rmdid and apg.apgstatus = 'A'
				inner join
					maismedicos.situacaoregistro sit ON sit.strcod = det.cs_ocorrencia
				where
					uni.uniid = {$uniid}
				".($arWhere ? ' and '.implode(' and ', $arWhere) : '')."
				order by
					tut.tuttipo desc, tut.tutnome";
		
		$rsSupervisores = $db->carregar($sql);
		unset($arDestinatarios);unset($arEmail);
		?>
		
		<?php if(!empty($rsTutores) && !empty($rsSupervisores)): ?>
			<?php $boExisteEnvio = true; ?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td class="subtituloCentro" colspan="2"><?php echo $rsSupervisores[0]['uninome'];?></td>
				</tr>
				<tr>
					<td class="subtituloDireita">Destinatário(s) <br/><?php echo count($rsTutores); ?> Tutor(es)</td>
					<td>
					<?php foreach($rsTutores as $j => $tutor): ?>
						<?php 
							$arDestinatarios[] = strtoupper($tutor['tutnome']).' &lt;'.$tutor['tutemail'].'&gt;';
// 							$arEmail[$j]['email']	= $tutor['tutemail'];
// 							$arEmail[$j]['nome']	= $tutor['tutnome'];
						?>
					<?php endforeach; ?>
					<?php if(!empty($arDestinatarios)) echo implode(', ', $arDestinatarios) ?>
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Conteúdo</td>
					<td>
					<?php 
					$arrCab = array("Nome","Função","Valor (R$)","Descrição","Instituição","Situação");
					$db->monta_lista_array($rsSupervisores, $arrCab, 10000000, 10, "N", "center", "");
					
					if($_REQUEST['confirmaEnvio'] == 'true'){

						$conteudo = montaConteudoEmail($arrCab, $rsSupervisores);
						
						if($_REQUEST['confirmaEnvio'] == 'true' && !empty($conteudo) && $email_enviado < 3){

							$arEmail[0]['email'] = 'wescley.lima@ebserh.gov.br';
							$arEmail[0]['nome'] = 'Wescley Lima';
							
							$arEmail[1]['email'] = 'henrique.couto@ebserh.gov.br';
							$arEmail[1]['nome'] = 'Henrique Couto';

							enviaEmailFolhaPagamento($conteudo, $arEmail);
							$email_enviado++;
							
							ver($email_enviado);
						}
					}
					?>
					</td>
				</tr>
			</table>
		<?php endif; ?>
		
		
		
	<?php endforeach; ?>
<?php endif; ?>

<?php if($boExisteEnvio): ?>
	<p><center>
		<input type="button" value="Confirmar envio" onclick="confirmaEnvioEmail()"/>
	</center></p>
<?php else: ?>
	<p><center>
		Não foram encontrados registros!
	</center></p>
<?php endif; ?>

<?php

function montaConteudoEmail($arCabecalho, $arDados)
{
	$txt = '';	
	if(!empty($arDados)){
		$txt = '<table style="width:100%" cellSpacing="3" cellPadding="3" align="center" style="font-size:11px;padding:3px;border-top: 2px solid #404040;border-bottom: 3px solid #dfdfdf;border-collapse: collapse;">
					<thead>
						<tr bgcolor="#E3E3E3"><th align="center" colspan="8">'.$arDados[0]['uninome'].'</th></tr>
						<tr bgcolor="#E3E3E3">
							<th align="left">'.implode('</th><th align="left">', $arCabecalho).'</th>
						</tr>
					</thead>
					<tbody>
					';
			foreach($arDados as $i => $dado){

				if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';

				$txt .= '<tr bgcolor="'.$marcado.'">
							<td align="left">'.implode('</td><td align="left">', $dado).'</td>
						 </tr>';
			}
		$txt .= '</tbody></table><br/>';
	
	}
	return $txt;
	
}

function enviaEmailFolhaPagamento($stConteudo, $arEmail)
{
	global $db;
	
	if(!empty($arEmail)){
	
		require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
		require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		
		$mensagem->From     	= 'noReply@mec.gov.br';
		$mensagem->FromName 	= 'SIMEC';
			
		foreach($arEmail as $email){
			$mensagem->AddAddress( $email['email'], $email['nome'] );
		}
		
		$mensagem->Subject 		= 'SIMEC - Mais Médicos - Folha de pagamento'; //assunto
		$mensagem->Body 		= $stConteudo; //conteudo
		$mensagem->IsHTML( true );
		$mensagem->Send();
	}
}
?>
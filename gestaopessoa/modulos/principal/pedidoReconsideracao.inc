<?php
$fdpcpf = $_SESSION['cpfavaliado'];

if( $_POST['requisicao'] == 'gravar' ){
	extract( $_POST );
	
	$ftprboletimserv = $ftprboletimserv ? $ftprboletimserv : 'null';
	$ftprdtboletimserv = $ftprdtboletimserv ? "'".formata_data_sql($ftprdtboletimserv)."'" : 'null';
	
	if( $ftprid ){
		$sql = "UPDATE gestaopessoa.ftpedidoreconsideracao SET 
				  fdpcpf = '$fdpcpf',
				  ftprboletimserv = $ftprboletimserv,
				  ftprdtboletimserv = $ftprdtboletimserv,
				  ftprconmettecdesatcaref = '$ftprconmettecdesatcaref',
				  ftprpontrecebida = '$ftprpontrecebida',
				  ftprpontsolicitada = '$ftprpontsolicitada',
				  ftprprodtrabalho = '$ftprprodtrabalho',
				  ftprpontrecebidaprodtrab = '$ftprpontrecebidaprodtrab',
				  ftprpontsolicprodtrab = '$ftprpontsolicprodtrab',
				  ftprcapautdesen = '$ftprcapautdesen',
				  ftprpontrecebcapdesen = '$ftprpontrecebcapdesen',
				  ftprpontsoliccapdesen = '$ftprpontsoliccapdesen',
				  ftprrelacinterpessoal = '$ftprrelacinterpessoal',
				  ftprpontrecebrelintpes = '$ftprpontrecebrelintpes',
				  ftprpontsolicrelintpes = '$ftprpontsolicrelintpes',
				  ftprtrabequipe = '$ftprtrabequipe',
				  ftprpontrecebtrabequipe = '$ftprpontrecebtrabequipe',
				  ftprpontsolictrabequipe = '$ftprpontsolictrabequipe',
				  ftprcomprtrabalho = '$ftprcomprtrabalho',
				  ftprpontrecebcomptrab = '$ftprpontrecebcomptrab',
				  ftprpontsoliccomptrab = '$ftprpontsoliccomptrab',
				  ftprcumpnorproccodatrcarg = '$ftprcumpnorproccodatrcarg',
				  ftprpontrecebcumpnorproccodatrcarg = '$ftprpontrecebcumpnorproccodatrcarg',
				  ftprpontsoliccumpnorproccodatrcarg = '$ftprpontsoliccumpnorproccodatrcarg',
				  flprparcerchefia = '$flprparcerchefia',
				  ftprpedido = '$ftprpedido',
				  ftprcienavaliador = '$ftprcienavaliador'
				 
				WHERE 
				  ftprid = $ftprid";
		$db->executar( $sql );
	} else {
		$sql = "INSERT INTO gestaopessoa.ftpedidoreconsideracao(fdpcpf, ftprboletimserv, ftprdtboletimserv, ftprconmettecdesatcaref, ftprpontrecebida, ftprpontsolicitada,
	  				ftprprodtrabalho, ftprpontrecebidaprodtrab, ftprpontsolicprodtrab, ftprcapautdesen, ftprpontrecebcapdesen, ftprpontsoliccapdesen, ftprrelacinterpessoal,
	  				ftprpontrecebrelintpes, ftprpontsolicrelintpes, ftprtrabequipe, ftprpontrecebtrabequipe, ftprpontsolictrabequipe, ftprcomprtrabalho, ftprpontrecebcomptrab,
	  				ftprpontsoliccomptrab, ftprcumpnorproccodatrcarg, ftprpontrecebcumpnorproccodatrcarg, ftprpontsoliccumpnorproccodatrcarg, flprparcerchefia, ftprpedido, ftprcienavaliador, anoreferencia) 
				VALUES ('$fdpcpf', $ftprboletimserv, $ftprdtboletimserv, '$ftprconmettecdesatcaref', '$ftprpontrecebida', '$ftprpontsolicitada',
	  				'$ftprprodtrabalho', '$ftprpontrecebidaprodtrab', '$ftprpontsolicprodtrab', '$ftprcapautdesen', '$ftprpontrecebcapdesen', '$ftprpontsoliccapdesen', '$ftprrelacinterpessoal',
	  				'$ftprpontrecebrelintpes', '$ftprpontsolicrelintpes', '$ftprtrabequipe', '$ftprpontrecebtrabequipe', '$ftprpontsolictrabequipe', '$ftprcomprtrabalho', '$ftprpontrecebcomptrab',
	  				'$ftprpontsoliccomptrab', '$ftprcumpnorproccodatrcarg', '$ftprpontrecebcumpnorproccodatrcarg', '$ftprpontsoliccumpnorproccodatrcarg', '$flprparcerchefia', '$ftprpedido', '$ftprcienavaliador', '{$_SESSION['exercicio']}')";
		$db->executar( $sql );
	}
	
	if( $db->commit() ){
		$db->sucesso( 'principal/pedidoReconsideracao' );
	} else {
		echo "<script>
				alert('Falha na operação');
				window.location.href = window.location; 
			</script>";
	}
	exit();
}

if( $fdpcpf ){
	$sql = "SELECT ftprid, fdpcpf, ftprboletimserv, ftprdtboletimserv, ftprconmettecdesatcaref, ftprpontrecebida, ftprpontsolicitada,
  				ftprprodtrabalho, ftprpontrecebidaprodtrab, ftprpontsolicprodtrab, ftprcapautdesen, ftprpontrecebcapdesen, ftprpontsoliccapdesen, ftprrelacinterpessoal,
  				ftprpontrecebrelintpes, ftprpontsolicrelintpes, ftprtrabequipe, ftprpontrecebtrabequipe, ftprpontsolictrabequipe, ftprcomprtrabalho, ftprpontrecebcomptrab,
  				ftprpontsoliccomptrab, ftprcumpnorproccodatrcarg, ftprpontrecebcumpnorproccodatrcarg, ftprpontsoliccumpnorproccodatrcarg, flprparcerchefia, ftprpedido, ftprcienavaliador
			FROM 
				gestaopessoa.ftpedidoreconsideracao WHERE fdpcpf = '$fdpcpf' AND anoreferencia = {$_SESSION['exercicio']}";
				
	$arrDados = $db->pegaLinha( $sql );
	$arrDados = $arrDados ? $arrDados : array();
	extract($arrDados);
	
	$sql = "SELECT ser.sercpf, ser.sernome, ser.sersiape, ser.sercargo, tls.tlssigla, ser.sercpfchefe
			FROM gestaopessoa.servidor ser
				left join gestaopessoa.tipolotacaoservidor tls on tls.tlsid = ser.tlsid and tls.tlsstatus = 'A'
			WHERE
				ser.sercpf = '$fdpcpf'
				AND ser.seranoreferencia = {$_SESSION['exercicio']}";
	$arrServidor = $db->pegaLinha( $sql );
	
	$sql = "SELECT DISTINCT ser.sercpf, ser.sernome, ser.sersiape, ser.sercargo, tls.tlssigla, ser.sercodigofuncao
			FROM gestaopessoa.servidor ser
				left join gestaopessoa.tipolotacaoservidor tls on tls.tlsid = ser.tlsid and tls.tlsstatus = 'A'
			WHERE
				ser.sercpf = '{$arrServidor['sercpfchefe']}'
				AND ser.seranoreferencia = {$_SESSION['exercicio']}";
	$arrChefe = $db->pegaLinha( $sql );
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>'; 
$db->cria_aba( $abacod_tela, $url, '' );

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

monta_titulo( 'Formulário de Pedido de Reconsideração', '' );
?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form id="formulario" name="formulario" method="post" action="">
<input type="hidden" name="requisicao" id="requisicao" value="">
<input type="hidden" name="ftprid" id="ftprid" value="<?=$ftprid; ?>">
<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
	<tr>
		<td style="text-align: center;" colspan="2">Avaliação de Desempenho Individual<br>
			<span style="font: bold; color: blue"><b>Ciclo Avaliativo: 01/11/2011 a 31/10/2012</b></span><br>
			Gratificação de Desempenho do Plano Geral de Cargos do Poder Executivo – GDPGPE<br>
			Decreto nº 7.133, de 19 de março de 2010<br><br><br>
		</td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" colspan="2"><b>I - Identificação do(a) Servidor(a)</b></td>
	</tr>
	<tr> 
		<td style="text-align: left; width: 50%"><b>Nome:</b> <?=$arrServidor['sernome']; ?></td> 
		<td style="text-align: left; width: 50%"><b>Matr&iacute;cula SIAPE:</b> <?=$arrServidor['sersiape']; ?></td> 
	</tr>
	<tr> 
		<td style="text-align: left;"><b>Cargo:</b> <?=$arrServidor['sercargo']; ?></td> 
		<td style="text-align: left;"><b>Fun&ccedil;&atilde;o:</b> <?=$arrServidor['sercodigofuncao']; ?></td> 
	</tr>
	<tr> 
		<td style="text-align: left;"><b>Unidade de Exerc&iacute;cio:</b> <?=$arrServidor['tlssigla']; ?></td> 
		<td style="text-align: left;"><b>Ramal:</b> <?=$arrServidor['']; ?></td> 
	</tr>
	<tr>
		<td class="subtituloEsquerda" colspan="2"><b>II - Identificação do(a) Avaliador(a)</b></td>
	</tr>
	<tr> 
		<td style="text-align: left; width: 50%"><b>Nome:</b> <?=$arrChefe['sernome']; ?></td> 
		<td style="text-align: left; width: 50%"><b>Matr&iacute;cula SIAPE:</b> <?=$arrChefe['sersiape']; ?></td> 
	</tr>
	<tr> 
		<td style="text-align: left;"><b>Cargo:</b> <?=$arrChefe['sercargo']; ?></td> 
		<td style="text-align: left;"><b>Fun&ccedil;&atilde;o:</b> <?=$arrChefe['sercodigofuncao']; ?></td> 
	</tr>
	<tr> 
		<td style="text-align: left;"><b>Unidade:</b> <?=$arrChefe['tlssigla']; ?></td> 
		<td style="text-align: left;"><b>Ramal:</b> <?=$arrChefe['']; ?></td> 
	</tr>
	</tbody>
	<tr>
		<td class="subtituloEsquerda" colspan="2"><b>III – Manifestação Expressa do(a) Servidor(a)</b></td>
	</tr>
	<tr>
		<td colspan="2"><span style="white-space: pre;">&nbsp;</span>Por intermédio deste documento venho requerer ao meu Chefe Imediato ou ao seu Substituto, 
		responsável pela minha Avaliação de Desempenho Individual referente à Gratificação de Desempenho do Plano Geral de Cargos do Poder Executivo – GDPGPE, 
		RECONSIDERAÇÃO do resultado final obtido na Avaliação Individual, 
		conforme publicação ocorrida no Boletim de Serviço nº <?=campo_texto('ftprboletimserv', 'N', 'S', 'Nº Boletim Serviço', 3, 10, '[#]', ''); ?> de
		 <? $ftprdtboletimserv = ($ftprdtboletimserv ? formata_data( $ftprdtboletimserv ) : "");
			echo campo_data2('ftprdtboletimserv', 'N', 'S','Data Inicio da Vigência', '', '', '', $ftprdtboletimserv, '', '', 'ftprdtboletimserv'); ?>, com base nas seguintes justificativas:<br><br>
		<span style="font-size: 10px">(Descrever o motivo da não concordância com a avaliação de desempenho, apontando por Fator de Avaliação)</span></td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" colspan="2">Conhecimento de métodos e técnicas para o desenvolvimento das atividades do cargo efetivo</td>
	</tr>
	<tr>
		<td colspan="2">
		<table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td>Justificativa:<br>
					<? echo campo_textarea('ftprconmettecdesatcaref', 'N', 'S', 'Justificativa', 200, 10, 4000); ?></td>
				<td valign="top">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=campo_texto('ftprpontrecebida', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontrecebida ); ?></td>
							<td><?=campo_texto('ftprpontsolicitada', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontsolicitada ); ?></td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" colspan="2">Produtividade no trabalho</td>
	</tr>
	<tr>
		<td colspan="2">
		<table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td>Justificativa:<br>
					<? echo campo_textarea('ftprprodtrabalho', 'N', 'S', 'Justificativa', 200, 10, 4000); ?></td>
				<td valign="top">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=campo_texto('ftprpontrecebidaprodtrab', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontrecebidaprodtrab ); ?></td>
							<td><?=campo_texto('ftprpontsolicprodtrab', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontsolicprodtrab ); ?></td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" colspan="2">Capacidade de auto desenvolvimento</td>
	</tr>
	<tr>
		<td colspan="2">
		<table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td>Justificativa:<br>
					<? echo campo_textarea('ftprcapautdesen', 'N', 'S', 'Justificativa', 200, 10, 4000); ?></td>
				<td valign="top">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=campo_texto('ftprpontrecebcapdesen', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontrecebcapdesen ); ?></td>
							<td><?=campo_texto('ftprpontsoliccapdesen', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontsoliccapdesen ); ?></td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" colspan="2">Relacionamento interpessoal</td>
	</tr>
	<tr>
		<td colspan="2">
		<table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td>Justificativa:<br>
					<? echo campo_textarea('ftprrelacinterpessoal', 'N', 'S', 'Justificativa', 200, 10, 4000); ?></td>
				<td valign="top">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=campo_texto('ftprpontrecebrelintpes', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontrecebrelintpes ); ?></td>
							<td><?=campo_texto('ftprpontsolicrelintpes', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontsolicrelintpes ); ?></td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" colspan="2">Trabalho em equipe</td>
	</tr>
	<tr>
		<td colspan="2">
		<table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td>Justificativa:<br>
					<? echo campo_textarea('ftprtrabequipe', 'N', 'S', 'Justificativa', 200, 10, 4000); ?></td>
				<td valign="top">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=campo_texto('ftprpontrecebtrabequipe', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontrecebtrabequipe ); ?></td>
							<td><?=campo_texto('ftprpontsolictrabequipe', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontsolictrabequipe ); ?></td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" colspan="2">Comprometimento com trabalho</td>
	</tr>
	<tr>
		<td colspan="2">
		<table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td>Justificativa:<br>
					<? echo campo_textarea('ftprcomprtrabalho', 'N', 'S', 'Justificativa', 200, 10, 4000); ?></td>
				<td valign="top">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=campo_texto('ftprpontrecebcomptrab', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontrecebcomptrab ); ?></td>
							<td><?=campo_texto('ftprpontsoliccomptrab', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontsoliccomptrab ); ?></td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" colspan="2">Cumprimento das normas de procedimento de conduta no desempenho das atribuições do cargo</td>
	</tr>
	<tr>
		<td colspan="2">
		<table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td>Justificativa:<br>
					<? echo campo_textarea('ftprcumpnorproccodatrcarg', 'N', 'S', 'Justificativa', 200, 10, 4000); ?></td>
				<td valign="top">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=campo_texto('ftprpontrecebcumpnorproccodatrcarg', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontrecebcumpnorproccodatrcarg ); ?></td>
							<td><?=campo_texto('ftprpontsoliccumpnorproccodatrcarg', 'N', 'S', '', 5, 3, '[#]', '', '', '', 0, '', '', $ftprpontsoliccumpnorproccodatrcarg ); ?></td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td colspan="2">Nestes termos, pede deferimento.</td>
	</tr>
	<tr>
		<td style="text-align: center; height: 100px" colspan="2">
		Local e Data: _______________, ____ de ____________ de <?=date('Y'); ?>.</td>
	</tr>
	<tr>
		<td style="text-align: center;" colspan="2">___________________________________<br> Assinatura do(a) Servidor(a) Avaliado(a)
		</td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" colspan="2">IV – Reconsideração da Chefia Imediata ou do Substituto</td>
	</tr>
	<tr>
		<td colspan="2">Parecer:<br>
		<? echo campo_textarea('flprparcerchefia', 'N', 'S', 'Parecer', 200, 10, 4000); ?></td>
	</tr>
	<tr>
		<td colspan="2"><b>Conclusão:</b></td>
	</tr>
	<tr>
		<td colspan="2">
			<table align="left" style="width: 100%" border="0" cellpadding="3" cellspacing="1">
			<tr>
				<td width="33.3%"><input type="radio" <?=(($ftprpedido == 'D') ? 'checked="checked"' : '') ?>  name="ftprpedido" id="ftprpedido" value="D">Pedido Deferido</td>
				<td width="33.3%"><input type="radio" <?=(($ftprpedido == 'P') ? 'checked="checked"' : '') ?>  name="ftprpedido" id="ftprpedido" value="P">Pedido Deferido Parcialmente</td>
				<td width="33.3%"><input type="radio" <?=(($ftprpedido == 'I') ? 'checked="checked"' : '') ?>  name="ftprpedido" id="ftprpedido" value="I">Pedido Indeferido</td>
			</tr>
			</table>
			</td>
	</tr>
	<tr>
		<td style="text-align: center; height: 100px" colspan="2">Local e Data: _______________, ____ de ____________ de <?=date('Y'); ?>.</td>
	</tr>
	<tr>
		<td style="text-align: center;" colspan="2">_____________________________________________<br> Assinatura e carimbo da Chefia Imediata ou Substituto</td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" colspan="2">V – Responsabilidade da Coordenação-Geral de Gestão de Pessoas</td>
	</tr>
	<tr>
		<td colspan="2">Ciência do(a) Avaliado(a) do resultado do Pedido de Reconsideração:</td>
	</tr>
	<tr>
		<td colspan="2">
		<table align="left" style="width: 100%" border="0" cellpadding="3" cellspacing="1">
			<tr>
				<td width="33.3%"><input type="radio" <?=(($ftprcienavaliador == 'CO') ? 'checked="checked"' : '') ?>  name="ftprcienavaliador" id="ftprcienavaliador" value="CO">Concordo</td>
				<td width="33.3%"><input type="radio" <?=(($ftprcienavaliador == 'DR') ? 'checked="checked"' : '') ?>  name="ftprcienavaliador" id="ftprcienavaliador" value="DR">Discordo, informando que irei interpor Recurso.</td>
				<td width="33.3%"><input type="radio" <?=(($ftprcienavaliador == 'DN') ? 'checked="checked"' : '') ?>  name="ftprcienavaliador" id="ftprcienavaliador" value="DN">Discordo, porém não vou interpor Recurso.</td>
			</tr>
		</table></td>
	</tr>
	<tr>
		<td style="text-align: center; height: 100px" colspan="2">Local e Data: _______________, ____ de ____________ de <?=date('Y'); ?>.</td>
	</tr>
	<tr>
		<td style="text-align: center;" colspan="2">_____________________________________________<br> Assinatura do(a) Servidor(a) Avaliado(a)<br><br></td>
	</tr>
	<tr>
		<td colspan="2" class="subtituloCentro">
			<input type="button" name="bt_gravar" value="Gravar Pedido" onclick="gravarPedido();">
			<input type="button" name="bt_imprimir" value="Imprimir Avaliação" onclick="imprimir();">
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function imprimir(){
	window.open("gestaopessoa.php?modulo=principal/imprimirPedidoReconsideracao&acao=A","Pedido",'scrollbars=yes,height=500,width=900,status=no,toolbar=no,menubar=no,location=no');
}
function gravarPedido(){
	var boVazio = false;
	
	$('#formulario input, #formulario textarea').each(function(){
		if( $(this).val() != '' && $(this).attr('readonly') != true && $(this).attr('type') != 'button' ){
			if( $(this).attr('type') == 'checkbox' ){
				if( $(this).attr('checked') == true ){
					boVazio = true;
				}
			} else {
				boVazio = true;
			}
		}
		
	});
	
	if( !boVazio ){
		alert('Existe campo obrigatório vazio!');
		return false;
	}
	
	$('#requisicao').val('gravar');
	$('#formulario').submit();
}
</script>
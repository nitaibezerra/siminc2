<?php
$fdpcpf = $_SESSION['cpfavaliado'];

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

$imagem = "<img style=\"cursor:pointer\" src=\"/imagens/check.jpg\" border=\"0\">";
?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	</head>
	<style type="">
		@media print {.notprint { display: none } .div_rolagem{display: none} .div_rol{display: 'none'} }        
        
		@media screen {.notscreen { display: none; }.div_rol{display: none;} }
                       
		.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
		.div_rol{ overflow-x: auto; overflow-y: auto; height: 50px;}
                       
	</style>
	<body>
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
		conforme publicação ocorrida no Boletim de Serviço nº <?=$ftprboletimserv; ?> de
		 <? $ftprdtboletimserv = ($ftprdtboletimserv ? formata_data( $ftprdtboletimserv ) : "");
			echo $ftprdtboletimserv; ?>, com base nas seguintes justificativas:<br><br>
		<span style="font-size: 10px">(Descrever o motivo da não concordância com a avaliação de desempenho, apontando por Fator de Avaliação)</span></td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" colspan="2">Conhecimento de métodos e técnicas para o desenvolvimento das atividades do cargo efetivo</td>
	</tr>
	<tr>
		<td colspan="2">
		<table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td valign="top" width="90%">Justificativa:<br>
					<? echo $ftprconmettecdesatcaref; ?></td>
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
							<td><?=($ftprpontrecebida ? $ftprpontrecebida : '0') ?> </td>
							<td><?=($ftprpontsolicitada ? $ftprpontsolicitada : '0') ?> </td>
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
				<td valign="top" width="90%">Justificativa:<br>
					<? echo $ftprprodtrabalho; ?></td>
				<td valign="top" width="10%">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=($ftprpontrecebidaprodtrab ? $ftprpontrecebidaprodtrab : '0') ?> </td>
							<td><?=($ftprpontsolicprodtrab ? $ftprpontsolicprodtrab : '0') ?> </td>
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
				<td valign="top" width="90%">Justificativa:<br>
					<? echo $ftprcapautdesen; ?></td>
				<td valign="top" width="10%">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=($ftprpontrecebcapdesen ? $ftprpontrecebcapdesen : '0') ?> </td>
							<td><?=($ftprpontsoliccapdesen ? $ftprpontsoliccapdesen : '0') ?> </td>
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
				<td valign="top" width="90%">Justificativa:<br>
					<? echo $ftprrelacinterpessoal; ?></td>
				<td valign="top" width="10%">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=($ftprpontrecebrelintpes ? $ftprpontrecebrelintpes : '0') ?> </td>
							<td><?=($ftprpontsolicrelintpes ? $ftprpontsolicrelintpes : '0') ?> </td>
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
				<td valign="top" width="90%">Justificativa:<br>
					<? echo $ftprtrabequipe; ?></td>
				<td valign="top" width="10%">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=($ftprpontrecebtrabequipe ? $ftprpontrecebtrabequipe : '0') ?> </td>
							<td><?=($ftprpontsolictrabequipe ? $ftprpontsolictrabequipe : '0') ?> </td>
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
				<td valign="top" width="90%">Justificativa:<br>
					<? echo $ftprcomprtrabalho; ?></td>
				<td valign="top" width="10%">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=($ftprpontrecebcomptrab ? $ftprpontrecebcomptrab : '0') ?> </td>
							<td><?=($ftprpontsoliccomptrab ? $ftprpontsoliccomptrab : '0') ?> </td>
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
				<td valign="top" width="90%">Justificativa:<br>
					<? echo $ftprcumpnorproccodatrcarg; ?></td>
				<td valign="top" width="10%">
					<table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="subtitulocentro" colspan="2">Pontuação</td>
						</tr>
						<tr>
							<td class="subtitulocentro">Recebida</td>
							<td class="subtitulocentro">Solicitada</td>
						</tr>
						<tr style="text-align: center;">
							<td><?=($ftprpontrecebcumpnorproccodatrcarg ? $ftprpontrecebcumpnorproccodatrcarg : '0') ?> </td>
							<td><?=($ftprpontsoliccumpnorproccodatrcarg ? $ftprpontsoliccumpnorproccodatrcarg : '0') ?> </td>
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
		<td colspan="2" valign="top">Parecer:<br>
		<? echo $flprparcerchefia; ?></td>
	</tr>
	<tr>
		<td colspan="2"><b>Conclusão:</b></td>
	</tr>
	<tr>
		<td colspan="2">
			<table align="left" style="width: 100%" border="0" cellpadding="3" cellspacing="1">
			<tr>
				<td width="33.3%" valign="top">(<?=(($ftprpedido == 'D') ? $imagem : '&nbsp;&nbsp;&nbsp;&nbsp;') ?>) Pedido Deferido </td>
				<td width="33.3%" valign="top">(<?=(($ftprpedido == 'P') ? $imagem : '&nbsp;&nbsp;&nbsp;&nbsp;') ?>) Pedido Deferido Parcialmente </td>
				<td width="33.3%" valign="top">(<?=(($ftprpedido == 'I') ? $imagem : '&nbsp;&nbsp;&nbsp;&nbsp;') ?>) Pedido Indeferido </td>
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
				<td width="33.3%" valign="top">(<?=(($ftprcienavaliador == 'CO') ? $imagem : '&nbsp;&nbsp;&nbsp;&nbsp;') ?>) Concordo </td>
				<td width="33.3%" valign="top">(<?=(($ftprcienavaliador == 'DR') ? $imagem : '&nbsp;&nbsp;&nbsp;&nbsp;') ?>) Discordo, informando que irei interpor Recurso. </td>
				<td width="33.3%" valign="top">(<?=(($ftprcienavaliador == 'DN') ? $imagem : '&nbsp;&nbsp;&nbsp;&nbsp;') ?>) Discordo, porém não vou interpor Recurso. </td>
			</tr>
		</table></td>
	</tr>
	<tr>
		<td style="text-align: center; height: 100px" colspan="2">Local e Data: _______________, ____ de ____________ de <?=date('Y'); ?>.</td>
	</tr>
	<tr>
		<td style="text-align: center;" colspan="2">_____________________________________________<br> Assinatura do(a) Servidor(a) Avaliado(a)<br><br></td>
	</tr>
</table>
<div id = "div_rolagem" class="notprint">
<table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td colspan="2" class="subtituloCentro">
			<input type="button" name="bt_imprimir" value="Imprimir" onclick="javascript: window.print();">
			<input type="button" name="bt_fechar" value="Fechar" onclick="javascript: window.close();">
			</td>
	</tr>
</table>	
</div>
</body>
</html>
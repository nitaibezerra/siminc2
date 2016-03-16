<?php


?>
	<table cellspacing="1" cellpadding="2" align="center" class="Tabela" width="100%" style="background-color: rgb(238, 238, 238);">
		<tbody>
		 	<?php if( $_REQUEST['opcao'] == 1 ){ ?>
			<tr>
		 		<td class="subtitulocentro" colspan="4" style="background-color: #d0d0d0;">Especificações da Iniciativa</td>
		 	</tr>
			<tr>
				<td colspan="4">
					<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
						<tbody>
							<tr style="background-color: #ffffff;">
						 		<td><b>Descrição</b></td>
						 		<td><b>QTD</b></td>
						 		<td><b>Valor Unitário</b></td>
						 		<td><b>Valor Total</b></td>
						 		<td><b>Valor Proponente</b></td>
						 		<td><b>Valor Concedente</b></td>
						 		<td><b>Data Inicial</b></td>
						 		<td><b>Data Final</b></td>
						 		<td><b>Iniciativas</b></td>
						 	</tr>
							<tr>
						 		<td>Construção do Bloco</td>
						 		<td><?=campo_texto('QTD','N','S','',1,1,'','','','','','','','1');?></td>
						 		<td><?=campo_texto('valun','N','S','',10,10,'','','','','','','','120.327,00');?></td>
						 		<td>120.327,00</td>
						 		<td><?=campo_texto('valprop','N','S','',10,10,'','','','','','','','20.327,00');?></td>
						 		<td><?=campo_texto('valconc','N','S','',10,10,'','','','','','','','100.000,00');?></td>
						 		<td>15/12/2009</td>
						 		<td>15/08/2010</td>
						 		<td>Obras e Instalações</td>
						 	</tr>
							<tr style="background-color: #ffffff;">
						 		<td>Construção de Salas</td>
						 		<td><?=campo_texto('QTD','N','S','',1,1,'','','','','','','','1');?></td>
						 		<td><?=campo_texto('valun','N','S','',10,10,'','','','','','','','50.000,00');?></td>
						 		<td>50.000,00</td>
						 		<td><?=campo_texto('valprop','N','S','',10,10,'','','','','','','','5.000,00');?></td>
						 		<td><?=campo_texto('valconc','N','S','',10,10,'','','','','','','','45.000,00');?></td>
						 		<td>15/12/2009</td>
						 		<td>15/08/2010</td>
						 		<td>Reforma</td>
						 	</tr>
							<tr>
						 		<td>Construção de Unidade Academica</td>
						 		<td><?=campo_texto('QTD','N','S','',1,1,'','','','','','','','1');?></td>
						 		<td><?=campo_texto('valun','N','S','',10,10,'','','','','','','','50.000,00');?></td>
						 		<td>50.000,00</td>
						 		<td><?=campo_texto('valprop','N','S','',10,10,'','','','','','','','5.000,00');?></td>
						 		<td><?=campo_texto('valconc','N','S','',10,10,'','','','','','','','45.000,00');?></td>
						 		<td>15/12/2009</td>
						 		<td>15/08/2010</td>
						 		<td>Reforma</td>
						 	</tr>
							<tr>
						 		<td><input type="button" value="Salvar"></td>
						 	</tr>
						</tbody>
				 	</table>
				 </td>
			</tr>
		 	<?php } else if( $_REQUEST['opcao'] == 2 ){ ?>
			<tr>
		 		<td class="subtitulocentro" colspan="4" style="background-color: #d0d0d0;">Beneficiários</td>
		 	</tr>
			<tr>
				<td colspan="4">
					<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
						<tbody>
							<tr style="background-color: #ffffff;">
						 		<td><b>Tipo</b></td>
						 		<td><b>Zona Rural</b></td>
						 		<td><b>Zona Urbana</b></td>
						 		<td><b>Total</b></td>
						 		<td><b>Iniciativas</b></td>
						 	</tr>
							<tr>
						 		<td>Escolas</td>
						 		<td><?=campo_texto('zonrl','N','S','',10,10,'','','','','','','','0');?></td>
						 		<td><?=campo_texto('zonurb','N','S','',10,10,'','','','','','','','10');?></td>
						 		<td>10</td>
						 		<td>Obras e Instalações</td>
						 	</tr>
							<tr style="background-color: #ffffff;">
						 		<td>Professores</td>
						 		<td><?=campo_texto('zonrl','N','S','',10,10,'','','','','','','','0');?></td>
						 		<td><?=campo_texto('zonurb','N','S','',10,10,'','','','','','','','30');?></td>
						 		<td>30</td>
						 		<td>Reforma</td>
						 	</tr>
							<tr>
						 		<td><input type="button" value="Salvar"></td>
						 	</tr>
						</tbody>
				 	</table>
				 </td>
			</tr>
		 	<?php } else if( $_REQUEST['opcao'] == 3 ){ ?>
			<tr>
		 		<td class="subtitulocentro" colspan="4" style="background-color: #d0d0d0;">Escolas Beneficiadas</td>
		 	</tr>
			<tr>
				<td colspan="4">
					<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
						<tbody>
							<tr style="background-color: #ffffff;">
						 		<td><b>Ação</b></td>
						 		<td><b>Código INEP</b></td>
						 		<td><b>Nome da Escola</b></td>
						 		<td><b>Alunos Beneficiados</b></td>
						 	</tr>
							<tr>
						 		<td><center><img src="/imagens/excluir.gif" style="cursor: pointer" onclick="" border=0 title="Excluir"></center></td>
						 		<td><?=campo_texto('inep','N','S','',15,15,'','','','','','','','121833');?></td>
						 		<td>Escola 1</td>
						 		<td><?=campo_texto('alu','N','S','',10,10,'','','','','','','','10');?></td>
						 	</tr>
							<tr style="background-color: #ffffff;">
						 		<td><center><img src="/imagens/excluir.gif" style="cursor: pointer" onclick="" border=0 title="Excluir"></center></td>
						 		<td><?=campo_texto('inep','N','S','',15,15,'','','','','','','','284300');?></td>
						 		<td>Escola 2</td>
						 		<td><?=campo_texto('alu','N','S','',10,10,'','','','','','','','30');?></td>
						 	</tr>
							<tr>
						 		<td><input type="button" value="Salvar"></td>
						 	</tr>
						</tbody>
				 	</table>
				 </td>
			</tr>
		 	<?php } ?>
		</tbody>
	</table>

</form>
</body>
<script type="text/javascript" src="js/reformulacao.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<html>
	<head>
		<title>Simec - Extrato Unidade de Recurso</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>	
		<?php
		monta_titulo('Extrato Unidade de Recurso','Lançamentos');
				
		$sql = "select 
					ug.ungdsc 
				from 
					evento.unidaderecurso ur
				inner join  
					public.unidadegestora ug ON ug.ungcod = ur.ungcod 
				where 
					ur.ureid = ".$_REQUEST['ureid'];
		
		$rsUnidade = $db->pegaLinha($sql);
		?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="subtituloDireita">Unidade de Recurso</td>
				<td><?php echo $rsUnidade['ungdsc'] ?></td>
			</tr>
		</table>
		<?php		
		
		$sql = "
                    select  to_char(uccdatalancamento::date,'DD/MM/YYYY'), 
                            uccdesclancamento,
                            uccvalorlancamento					
                    from evento.unidadecontacorrente
                    where  ureidpai = ".$_REQUEST['ureid']." or ureidfilho = ".$_REQUEST['ureid']." 
                    order by uccdatalancamento DESC
                ";		
		$cabecalho = array('Data','Descrição','<div style="text-align:right">Valor (R$)</div>');
		$db->monta_lista($sql, $cabecalho, 10, 10, 'N', '', '', 'formExtrato', array('100px', null, '130px'));
		?>
	</body>
</html>
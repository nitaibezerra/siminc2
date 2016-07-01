<?php 

if($_REQUEST['requisicao'] == 'gerarPdf'){
	
	echo '<style>
	
			.subtitulodireita{
			    FONT-SIZE: 8pt;
			    COLOR: black;
			    FONT-FAMILY: Arial, Verdana;
			    TEXT-ALIGN: right;
				BACKGROUND-COLOR: #dcdcdc;
			}
			.tabela{
			    FONT-SIZE: xx-small;
			    BORDER-RIGHT: #cccccc 1px solid;
			    BORDER-TOP: #cccccc 1px solid;
			    BORDER-LEFT: #cccccc 1px solid;
			    BORDER-BOTTOM: #cccccc 1px solid;
				TEXT-DECORATION: none;
				WIDTH: 95%;
				TEXT-COLOR: #000000;
			}		
			.subtituloCentro{
			    FONT-WEIGHT: bold;
			    FONT-SIZE: 8pt;
			    COLOR: black;
			    FONT-FAMILY: Arial, Verdana;
			    BACKGROUND-COLOR: #f0f0f0;
			    TEXT-ALIGN: center
			}	
			table.listagem{
				font-size: 11px;
				padding: 3px;			
				border-top: 2px solid #404040;
				border-bottom: 3px solid #dfdfdf;			
				border-collapse: collapse;
			}	
			table.listagem P{
				font-size: 11px;
				margin: 0px;
				padding: 3px;		
			}	
			table.listagem P.marcado{color: #D90000;}

			table.listagem IMG{margin-left: 0px;}
				
			table.listagem thead{background-color: #E3E3E3;}
		
			table.listagem thead P{color: #6A6A6A;}

			table.listagem thead P B{color: #D90000;}
	
			table.listagem thead td{
				border-right: 1px solid white;
				padding: 2px;
				padding-left: 3px;
			}
			
			table.listagem thead td.order{
				background-color: #133368;
				border-right: 1px solid #ff9000;
			}

			table.listagem thead td A, 
			table.listagem thead td A:visited{
				color: #6A6A6A;
			}

			table.listagem thead td A:hover{color: red;}

			table.listagem thead td.order A{color: white;}

			table.listagem thead td.order A:visited{color: #BFBFBF;}
	
			table.listagem tbody td{
				border-right: 1px solid #f1f1f1;
				border-left: 1px solid white;
				border-collapse: collapse;
				border: 1px solid #ffffff;
			}

			table.listagem tbody td P A{color: #133368;}

			table.listagem tbody td A:visited{color: black;}

			table.listagem tbody td A:hover, 
			table.listagem tbody td A:visited:hover{color: #E47100;}

			table.listagem tbody td P A.alerta, 
			table.listagem tbody td P A.alerta:visited{
				color: #E47100;
				font-weight: bold;
			}

			table.listagem tbody td P A.alerta:hover, 
			table.listagem tbody td P A.alerta:visited:hover{
				color: black;
				font-weight: bold;
				text-decoration: none;
			}

			table.listagem tbody TR.marcado{background-color: #f5f5f5;}
			
		  </style>';
	
}else{
	
	include  APPRAIZ."includes/cabecalho.inc";
	print "<br/>";	
	
}

monta_titulo("Recursos de Capital e Custeio Aplicado", "");

/*$sql = "select 
			emi.emiid,
			ede.estuf,
			ent.entnome,	
			tjo.etjjornada,	
			est.estdescricao,
			(SELECT
				coalesce(sum(cen.emcquantidadealunos),0) as total 
			FROM 
				em.emicenso cen
			WHERE 
				cen.entid::integer = ent.entid)	as alunos,
			(SELECT 
				sum(rcc.emrcusteio+rcc.emrcapital) as total
			FROM 
				em.emireccusteiocapital rcc
			WHERE 
				rcc.emrjornada = tjo.etjjornada 
			AND 
				(SELECT
					coalesce(sum(cen.emcquantidadealunos),0) as total 
				FROM 
					em.emicenso cen
				WHERE 
					cen.entid::integer = ent.entid) BETWEEN rcc.emrqtdinialunos 
			AND 
				rcc.emrqtdfinalunos) as valor
		from 
			em.emiensinomedioinovador emi
		inner join 
			entidade.entidade ent on ent.entid = emi.entid
		inner join 
			entidade.endereco ede on ede.entid = ent.entid
		inner join 
			em.emitipojornada tjo on tjo.etjid = emi.etjid
		inner join
			territorios.estado est on upper(est.estuf) = upper(ede.estuf)
		order by
			ede.estuf, ent.entnome";*/
$sql = "
	select distinct emi.emiid,
	       ede.estuf,
	       ent.entnome,
	       tjo.etjjornada,
	       est.estdescricao,
	       emi.entid
	FROM 
		em.emiensinomedioinovador emi
	INNER JOIN entidade.entidade  ent on ent.entid        = emi.entid
	INNER JOIN entidade.endereco  ede on ede.entid        = ent.entid
	INNER JOIN em.emitipojornada  tjo on tjo.etjid        = emi.etjid
	INNER JOIN territorios.estado est on upper(est.estuf) = upper(ede.estuf)
	INNER JOIN em.emigap          gap on gap.emiid        = emi.emiid
	WHERE
		ede.estuf = 'BA'
		AND emi.emistatus = 'A'
	ORDER BY ede.estuf, ent.entnome";

$rsDemonstrativo = $db->carregar($sql);

$arUf = array();
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<thead>
		<tr>
			<th>&nbsp;</th>
			<th colspan="9">Quantitativo de alunos na jornada de 5 horas</th>
			<th colspan="9">Quantitativo de alunos na jornada de 7 horas</th>
			<th>&nbsp;</th>
		</tr>
		<tr>
			<th>Escola</th>
			<th>até 100</th>
			<th>101 a 300</th>
			<th>301 a 500</th>
			<th>501 a 700</th>
			<th>701 a 900</th>
			<th>901 a 1100</th>
			<th>1101 a 1300</th>
			<th>1301 a 1400</th>
			<th>acima de 1400</th>
			<th>até a 100</th>
			<th>101 a 300</th>
			<th>301 a 500</th>
			
			<th>501 a 700</th>
			<th>701 a 900</th>
			<th>901 a 1100</th>
			<th>1101 a 1300</th>
			<th>1301 a 1400</th>
			<th>acima de 1400</th>
			<th>TOTAL</th>			
		</tr>
	</thead>
	<tbody>
		<?php 
		$jonada5valor1 = 0;
		$jonada5valor2 = 0;
		$jonada5valor3 = 0;
		$jonada5valor4 = 0;
		$jonada5valor5 = 0;
		$jonada5valor6 = 0;
		$jonada5valor7 = 0;
		$jonada5valor8 = 0;
		$jonada5valor9 = 0;

		$jonada7valor1 = 0;
		$jonada7valor2 = 0;
		$jonada7valor3 = 0;
		$jonada7valor4 = 0;
		$jonada7valor5 = 0;
		$jonada7valor6 = 0;
		$jonada7valor7 = 0;
		$jonada7valor8 = 0;
		$jonada7valor9 = 0;
		
		$x=0;
		$y=0;
		
		$final = count($rsDemonstrativo);
		?>
		<?php foreach($rsDemonstrativo as $dados): ?>
			<?php ++$y; ?>
			<?php if(!in_array($dados['estuf'], $arUf) && $x != 0): ?>
				<tr>
					<td><b>TOTAL:</b></td>
					<td><?php echo '<b>'.number_format($jonada5valor1, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor2, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor3, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor4, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor5, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor6, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor7, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor8, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor9, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor1, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor2, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor3, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor4, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor5, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor6, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor7, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor8, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor9, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($total, 2, ",", ".").'</b>'; ?></td>					
				</tr>
			<?php endif; ?>
			<?php if(!in_array($dados['estuf'], $arUf)): ?>
				<?php $arUf[$dados['estuf']] = $dados['estuf']; ?>
				<tr>
					<td colspan="20" class="subtituloEsquerda"><?php echo $dados['estdescricao'].' - '.$dados['estuf']; ?></td>
				</tr>
				<?php 
				
				$x++; 
				$total = 0;

				$jonada5valor1 = 0;
				$jonada5valor2 = 0;
				$jonada5valor3 = 0;
				$jonada5valor4 = 0;
				$jonada5valor5 = 0;
				$jonada5valor6 = 0;
				$jonada5valor7 = 0;
				$jonada5valor8 = 0;
				$jonada5valor9 = 0;
		
				$jonada7valor1 = 0;
				$jonada7valor2 = 0;
				$jonada7valor3 = 0;
				$jonada7valor4 = 0;
				$jonada7valor5 = 0;
				$jonada7valor6 = 0;
				$jonada7valor7 = 0;
				$jonada7valor8 = 0;
				$jonada7valor9 = 0;
				
				
				
			endif;
			
			$sqlAlunos = "
				SELECT coalesce(sum(cen.emcquantidadealunos),0) as total
		        FROM em.emicenso cen
		        WHERE cen.entid::integer = ".$dados['entid']."
			";
			$dadosAlunos = $db->pegaLinha($sqlAlunos);
			
			$sqlValores = "
				SELECT sum(rcc.emrcusteio+rcc.emrcapital) as total
		        FROM em.emireccusteiocapital rcc
		        WHERE  rcc.emrjornada = ".$dados['etjjornada']."
		        AND   (SELECT coalesce(sum(cen.emcquantidadealunos),0) as total
		               FROM em.emicenso cen
		               WHERE cen.entid::integer = ".$dados['entid'].") BETWEEN rcc.emrqtdinialunos
		                                                               AND     rcc.emrqtdfinalunos
			";
			$dadosValores = $db->pegaLinha($sqlValores);
			//dbg($dadosValores);die;
			
			?>
			<tr>
				<td><?php echo $dados['entnome']; ?></td>
				<td><?php if($dadosAlunos['total'] <= 100 && $dados['etjjornada'] == 5){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada5valor1 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 101 && $dadosAlunos['total'] <= 300  && $dados['etjjornada'] == 5){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada5valor2 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 301 && $dadosAlunos['total'] <= 500  && $dados['etjjornada'] == 5){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada5valor3 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 501 && $dadosAlunos['total'] <= 700  && $dados['etjjornada'] == 5){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada5valor4 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 701 && $dadosAlunos['total'] <= 900  && $dados['etjjornada'] == 5){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada5valor5 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 901 && $dadosAlunos['total'] <= 1100  && $dados['etjjornada'] == 5){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada5valor6 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 1101 && $dadosAlunos['total'] <= 1300  && $dados['etjjornada'] == 5){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada5valor7 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 1301 && $dadosAlunos['total'] <= 1400  && $dados['etjjornada'] == 5){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada5valor8 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] > 1400 && $dados['etjjornada'] == 5){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada5valor9 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] <= 100 && $dados['etjjornada'] == 7){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada7valor1 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 101 && $dadosAlunos['total'] <= 300  && $dados['etjjornada'] == 7){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada7valor2 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 301 && $dadosAlunos['total'] <= 500  && $dados['etjjornada'] == 7){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada7valor3 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 501 && $dadosAlunos['total'] <= 700  && $dados['etjjornada'] == 7){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada7valor4 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 701 && $dadosAlunos['total'] <= 900  && $dados['etjjornada'] == 7){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada7valor5 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 901 && $dadosAlunos['total'] <= 1100  && $dados['etjjornada'] == 7){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada7valor6 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 1101 && $dadosAlunos['total'] <= 1300  && $dados['etjjornada'] == 7){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada7valor7 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] >= 1301 && $dadosAlunos['total'] <= 1400  && $dados['etjjornada'] == 7){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada7valor8 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php if($dadosAlunos['total'] > 1400 && $dados['etjjornada'] == 7){ echo number_format($dadosValores['total'], 2, ",", "."); $jonada7valor9 += $dadosValores['total']; }else{ echo '&nbsp;';} ?></td>
				<td><?php echo '<b>'.number_format($dadosValores['total'], 2, ",", ".").'</b>'; $total += $dadosValores['total']; ?></td>				
			</tr>
			<?php if($y == $final): ?>
				<tr>
					<td><b>TOTAL:</b></td>
					<td><?php echo '<b>'.number_format($jonada5valor1, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor2, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor3, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor4, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor5, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor6, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor7, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor8, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada5valor9, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor1, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor2, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor3, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor4, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor5, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor6, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor7, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor8, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($jonada7valor9, 2, ",", ".").'</b>'; ?></td>
					<td><?php echo '<b>'.number_format($total, 2, ",", ".").'</b>'; ?></td>					
				</tr>
			<?php endif; ?>
		<?php endforeach; ?>
		
	</tbody>
</table>
<?php 
if($_REQUEST['requisicao'] == 'gerarPdf'){
	
	$html = ob_get_contents();
	ob_clean();
	
	$content = http_build_query( array('conteudoHtml'=> utf8_encode($html) ) );
	$context = stream_context_create(array( 'http'    => array(
							                'method'  => 'POST',
							                'content' => $content ) ));
	
	$contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context );
	
	header('Content-Type: application/pdf');
	header("Content-Disposition: attachment; filename=plano_atendimento_global");
	
	echo $contents;
	exit;
}
?>
<script>
function gerarPdf()
{
	document.location.href = 'em.php?modulo=relatorio/demonstrativoRecurso&acao=A&requisicao=gerarPdf';
}
</script>
<center>
<p><input type="button" value="Gerar PDF" onclick="gerarPdf()"/></p>
</center>
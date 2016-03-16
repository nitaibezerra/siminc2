<?php
include  "_funcoes_novasturmas.php";

$_REQUEST['muncod'] = ( $_REQUEST['muncod'] ? $_REQUEST['muncod'] : $_SESSION['proinfantil']['muncod'] ); 
$_REQUEST['turid'] = $_REQUEST['turma'];

$turid = $_REQUEST['turid'];

$sql = "SELECT DISTINCT
			t.turdsc,
			coalesce(to_char(na.anadatainclusao,'DD/MM/YYYY HH24:MI:SS'), 'Não informado.') as data_parecer,
			na.anadatainclusao,
			coalesce(uss.usunome, 'Não informado.') as responsavel_parecer,
		    na.anaparecer,
		    na.anatipo,
		    nd.diltexto,
		    to_char(nd.dildata, 'DD/MM/YYYY HH24:MI:SS') as data,
            usu.usunome,
		    case when na.anatipo = 'A' then 'Aprovado'
		        when na.anatipo = 'I' then 'Indeferida'
		        when na.anatipo = 'D' then 'Diligenciada'
		    else '<span style=\"color: red;\">Não Analisado</span>' end as tipoanalise
		FROM
			proinfantil.turma t
		INNER JOIN proinfantil.novasturmasworkflowturma 	ntw ON ntw.turid = t.turid
		INNER JOIN proinfantil.analisenovasturmasaprovacao 	na 	ON na.turid = t.turid --AND anastatus = 'A'
		LEFT  JOIN seguranca.usuario						uss ON uss.usucpf = na.usucpf
		LEFT JOIN proinfantil.novasturmasdiligencia 		nd 
        	INNER JOIN seguranca.usuario 	usu ON usu.usucpf = nd.usucpf
        														ON nd.turid = t.turid AND nd.anaid = na.anaid AND nd.dilstatus = 'A'
		WHERE
			t.turid = $turid
		ORDER BY
			na.anadatainclusao desc";

$arrTurma = $db->carregar($sql);
$arrTurma = $arrTurma ? $arrTurma : array();

$nome_turma = "Turma: ".$arrTurma[0]['turdsc'];

monta_titulo($nome_turma, '');
?>
<html>
<head>
	<script type="text/javascript" src="/includes/estouvivo.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript">
	$(document).ready(function(){
		
	});
	</script>
</head>

<body>
<?php cabecalhoTurma( $turid ); ?>
<table border="0" class="tabela" style="width: 95%" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td>
			<?php 
			foreach ($arrTurma as $v) {
				?>
				<table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<tr>
					<td class="subtituloDireita" width="30%"><b>Tipo de Parecer:</b></td>
					<td><?php echo $v['tipoanalise']; ?></td>
				</tr>
				<tr>
					<td class="subtituloDireita" width="30%"><b>Data do Parecer:</b></td>
					<td><?php echo $v['data_parecer']; ?></td>
				</tr>
				<tr>
					<td class="subtituloDireita" width="30%"><b>Responsável pelo Parecer:</b></td>
					<td><?php echo $v['responsavel_parecer']; ?></td>
				</tr>
				<tr>
					<td class="subtituloDireita" width="30%"><b>Parecer:</b></td>
					<td><?php echo $v['anaparecer']; ?></td>
				</tr>
				<?if( $v['anatipo'] == 'D' ){ ?>
					<tr>
						<td class="subtituloDireita" width="30%"><b>Resposta da Diligência:</b></td>
						<td><?php echo $v['diltexto']; ?></td>
					</tr>
					<tr>
						<td class="subtituloDireita" width="30%"><b>Data da Resposta:</b></td>
						<td><?php echo $v['data']; ?></td>
					</tr>
					<tr>
						<td class="subtituloDireita" width="30%"><b>Responsável pela Resposta:</b></td>
						<td><?php echo $v['usunome']; ?></td>
					</tr>
				<?} ?>
				</table>
				<br>
				<?php 
			}
			?>		
		</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center"><input type="button" name="btnFechar" value="Fechar" onclick="javascript: window.close();"></td>
	</tr>
</table>
</body>
</html>
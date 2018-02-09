<?php

validaSessionPTA( $_SESSION["emenda"]["ptrid"] );
validaSessionPTA( $_SESSION['emenda']['federal'] );

include APPRAIZ . 'includes/workflow.php';
if( $_SESSION['emenda']['tpopup'] != 'popup' ){
	include  APPRAIZ."includes/cabecalho.inc";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}

$emendaFederal = $_SESSION['emenda']['federal'];


$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );

if( $_SESSION['emenda']['federal'] == 'S' ){
	$label = 'Nota de Crédito';
} else {
	$label = 'Nº Empenho Original';
}

print "<br/>";
echo exibeInstituicaoBenefiada( $_SESSION["emenda"]["enbid"] );
print "<br/>";
montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );

monta_titulo( 'Histórico de Movimentação das Análise do PTA ', '');

echo cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptrid"]);
$docid = criarDocumento( $_SESSION["emenda"]["ptrid"] );
$documento = wf_pegarDocumento( $docid );
$atual = wf_pegarEstadoAtual( $docid );
$historico = wf_pegarHistorico( $docid );

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script language="JavaScript" src="../../../includes/funcoes.js"></script>
		<!-- biblioteca javascript local -->
		<script type="text/javascript">
			
			IE = !!document.all;
			
			function exebirOcultarComentario( docid )
			{
				id = 'comentario' + docid;
				div = document.getElementById( id );
				if ( !div )
				{
					return;
				}
				var display = div.style.display != 'none' ? 'none' : 'table-row';
				if ( display == 'table-row' && IE == true )
				{
					display = 'block';
				}
				div.style.display = display;
			}
			
		</script>
	</head>
	<body topmargin="0" leftmargin="0">
		<form action="" method="post" name="formulario">
			<table class="listagem" cellspacing="0" cellpadding="3" align="center" width="95%">
				<thead>
					<tr>
						<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
							<b style="font-size: 10pt;">Acompanhamento<br/></b>
						</td>
					</tr>
				</thead>
				<?
				$sql = "SELECT 
						  (case when ptr.ptrnumconvenio is not null then ptr.ptrnumconvenio ||'/'|| ptr.ptranoconvenio else '-' end) AS convenioFNDE,
						  (case when ptr.ptrnumprocessoempenho is not null then ptr.ptrnumprocessoempenho else '-' end) as ptrnumprocessoempenho, 
						  (case when exf.exfnumempenhooriginal is not null then sum(exf.exfvalor) else '0' end) as valorempenhado,
						  (case when exf.exfnumempenhooriginal is not null then exf.exfnumempenhooriginal else '-' end) as exfnumempenhooriginal,
						  (case when pmc.pmcnumconveniosiafi is not null then pmc.pmcnumconveniosiafi else '-' end) as pmcnumconveniosiafi
						  
						  
						FROM
						  emenda.planotrabalho ptr  
						  LEFT JOIN emenda.ptminutaconvenio pmc on pmc.ptrid = ptr.ptrid
							and pmc.pmcstatus = 'A'
						  LEFT JOIN emenda.execucaofinanceira exf on ptr.ptrid = exf.ptrid
							and exf.exfstatus = 'A' 
							
						WHERE
						  	ptr.ptrid = ".$_SESSION['emenda']['ptrid']."
							AND ptr.ptrexercicio = ".$_SESSION["exercicio"]."
						Group by 
						ptr.ptrnumconvenio,
						ptr.ptranoconvenio,
						ptr.ptrnumprocessoempenho, 
						exf.exfvalor,
						exf.exfnumempenhooriginal,
						pmc.pmcnumconveniosiafi";



			$dadosPlanoTrabalho = $db->carregar($sql);
			
			
			if($dadosPlanoTrabalho) {
			?>
				<tr>
					<td style="text-align: left; width:20.0%;" class="SubTituloEsquerda">Processo:</td>
					<td colspan=3 style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class="SubTituloDireita"><?php echo (( $dadosPlanoTrabalho[0]['ptrnumprocessoempenho'])?substr( $dadosPlanoTrabalho[0]['ptrnumprocessoempenho'],0,5)."." .substr( $dadosPlanoTrabalho[0]['ptrnumprocessoempenho'],5,6)."/".substr( $dadosPlanoTrabalho[0]['ptrnumprocessoempenho'],11,4)."-".substr( $dadosPlanoTrabalho[0]['ptrnumprocessoempenho'],15,2):"");?></td>
				</tr>
				<?if( $emendaFederal == 'N' ){ ?>
				<tr>
					<td style="text-align: left; width:20.0%;" class="SubTituloEsquerda">Convênio FNDE:</td>
					<td style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class="SubTituloDireita"><?php echo $dadosPlanoTrabalho[0]['conveniofnde'];?></td>
					<td style="text-align: left; width:20.0%;" class="SubTituloEsquerda">Convênio SIAFI:</td>
					<td style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class="SubTituloDireita"><?php echo $dadosPlanoTrabalho[0]['pmcnumconveniosiafi'];?></td>
				</tr>
				<?} ?>
				<tr> <td colspan=4><br>
				
				<table cellspacing="0" cellpadding="0" align="left" width="100%">
					<tr>
				 		<td colspan="2" class="subtitulocentro">Dados da execução orçamentária</td>
				 	</tr>
					<tr>
						<td style="text-align: left;" class="SubTituloEsquerda">Valor</td>
						<td style="text-align: left;" class="SubTituloEsquerda"><?=$label; ?></td>
					</tr>
					<?php	
					foreach($dadosPlanoTrabalho as $dadospta){ ?>
	    				<tr>
							<td style="width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class="SubTituloDireita"><?php echo $dadospta['valorempenhado'];?></td>
							<td style="width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class="SubTituloDireita"><?php echo $dadospta['exfnumempenhooriginal'];?></td>
						</tr>
						<?php
				    }
					?>
		    	</table>
		    	</td></tr>
			<?php
			} else {
				echo '<tr>
				    	<td style="color: red">Dados não encontrados!</td>
				    </tr>';
			}
		    ?>
			
			</table>
			<br/>
			
			<table class="listagem" cellspacing="0" cellpadding="3" align="center" width="95%">
				<thead>
					<tr>
						<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
							<b style="font-size: 10pt;">Histórico de Tramitações<br/></b>
							<div><?php
									$docdsc = explode( '-', $documento['docdsc'] );
									echo $docdsc[0]; ?></div>
						</td>
					</tr>
					<?php if ( count( $historico ) ) : ?>
						<tr>
							<td style="width: 20px;"><b>Seq.</b></td>
							<td style="width: 200px;"><b>Onde Estava</b></td>
							<td style="width: 200px;"><b>O que aconteceu</b></td>
							<?if( !in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) && !in_array( AUTOR_EMENDA, $arPerfil ) ){ ?>
								<td style="width: 90px;"><b>Quem fez</b></td>
							<?} ?>
							<td style="width: 120px;"><b>Quando fez</b></td>
							<td style="width: 17px;">&nbsp;</td>
						</tr>
					<?php endif; ?>
				</thead>
				<?php $i = 1; ?>
				<?php foreach ( $historico as $item ) : ?>
					<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
					<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
						<td align="right"><?=$i?>.</td>
						<td style="color:#008000;">
							<?php echo $item['esddsc']; ?>
						</td>
						<td valign="middle" style="color:#133368">
							<?php echo $item['aeddscrealizada']; ?>
						</td>
						<?if( !in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) && !in_array( AUTOR_EMENDA, $arPerfil ) ){ ?>
						<td style="font-size: 6pt;">
							<?php echo $item['usunome']; ?>
						</td>
						<?} ?>
						<td style="color:#133368">
							<?php echo $item['htddata']; ?>
						</td>
						<td style="color:#133368; text-align: center;">
							<?php if( $item['cmddsc'] ) : ?>
								<img
									align="middle"
									style="cursor: pointer;"
									src="http://<?php echo $_SERVER['SERVER_NAME'] ?>/imagens/restricao.png"
									onclick="exebirOcultarComentario( '<?php echo $i; ?>' );"
								/>
							<?php endif; ?>
						</td>
					</tr>
					<tr id="comentario<?php echo $i; ?>" style="display: none;" bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
						<td colspan="6">
							<div >
								<?php echo simec_htmlentities( $item['cmddsc'] ); ?>
							</div>
						</td>
					</tr>
					<?php $i++; ?>
				<?php endforeach; ?>
				<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
				<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
					<td style="text-align: right;" colspan="6">
						Estado atual: <span style="color:#008000;"><?php echo $atual['esddsc']; ?></span>
					</td>
				</tr>
			</table>
			<br/>
		</form>
	</body>
</html>
<?php 

if($_REQUEST['requisicao'] == 'carregaComboPortariasRel'){
	$ano = $_REQUEST['ano'];
	$mes = $_REQUEST['mes'];
	
	//$semestre = 1;
	//if ($mes > 6)
	//	$semestre = 2;
	
	$sql = "SELECT 	DISTINCT	p.ppeid AS codigo, 
								p.ppenumero AS descricao 
			FROM	academico.portariaprofequival  as p
			WHERE	p.ppeano = {$ano} 
			--AND	p.ppesemestre = {$semestre}";

	$db->monta_combo('portaria', $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'portaria');
	die;
}

function montaSqlProfEquivalente($filtros = array())
{
	if(!empty($filtros)){
		extract($filtros);
	} else {
		extract($_POST);
	}
	
	if($ano) $arWhere[] = "ppe.ppeano = '{$ano}'";
	if($portaria) $arWhere[] = "ppe.ppeid = {$portaria}";
	if($mes) $arWhere[] = "mpemes = {$mes}";
	
	$sql = "SELECT
			UPPER(entsig) as codigo,				
			UPPER(entnome) as orgao, 
			upper(mun.mundescricao) as municipio, 
			upper(mun.estuf) as uf,
			--mpeano,
			--mpemes,
			coalesce(mpevlr20h,0) as mpevlr20h,
			coalesce(mpevlr40h,0) as mpevlr40h,
			coalesce(mpevlrdedexclusiva,0) as mpevlrdedexclusiva,
			(mpevlr20h+mpevlr40h+mpevlrdedexclusiva) as total,
			coalesce(mpevlrsubstituto,0) as mpevlrsubstituto,
			coalesce(mpevlrvisitante,0) as mpevlrvisitante,
			coalesce(mpevlrvagos,0) as mpevlrvagos,
			coalesce(mpevlrbcequiv,0) as mpevlrbcequiv,
			coalesce(ptvvalor,0) as ptvvalor,
			(coalesce(ptvvalor,0)) - ( (coalesce(mpevlr20h,0)*0.58) + (coalesce(mpevlr40h,0)*1) + (coalesce(mpevlrdedexclusiva,0)*1.7) + (coalesce(mpevlrsubstituto,0)*1) + (coalesce(mpevlrvisitante,0)*1.7) ) as portmpmec
		FROM
			entidade.entidade e 
		INNER JOIN
			entidade.funcaoentidade ef ON ef.entid = e.entid
		LEFT JOIN
			academico.portariavalor pv ON pv.entid = e.entid  
		LEFT JOIN
			entidade.endereco ed ON ed.entid = e.entid
		LEFT JOIN
			territorios.municipio mun ON mun.muncod = ed.muncod
		LEFT JOIN
			academico.movprofequivalente mpe ON mpe.entid = e.entid AND mpestatus = 'A'				
		LEFT JOIN 
			academico.portariaprofequival ppe ON pv.ppeid = ppe.ppeid
		WHERE
			e.entstatus = 'A' AND ef.funid  in ('12')
		".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."		
		ORDER BY
			 e.entsig, e.entnome";
	
	return $sql;
}

function montarPortaria($ppeid){
	global $db;
	
	$sql = "SELECT 	'Port. MP/MEC ' || ppenumero || '/' || ppeano AS descricao_portaria 
			FROM 	 academico.portariaprofequival
			WHERE	 ppeid = {$ppeid}";
	$rs = $db->pegaUm($sql);	
	return $rs;
}

if($_REQUEST['requisicao'] == 'gerarXls'){

	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_SIC_RelatorioGeral_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_SIC_RelatorioGeral_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
		
	$arDados = montaSqlProfEquivalente( $_REQUEST );
	$arCabecalho = array('Código', 'Órgão', 'Município', 'UF', '20h', '40h', 'DE', 'Total', 'Subst.', 'Visit.', 'Vagos', 'BancoPEqv', 'Port. MP/MEC 440/2011', 'Saldo');
	
	$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%','');	
	die;
}

include_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";


if($_POST) extract($_POST);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script><!--

	$(function(){
		
		
		$('#btnImprimir').click(function(){

			var ano = $('#ano').val();
			var mes = $('#mes').val();
			var portaria = $('#portaria').val();
			
			if( portaria && ano && mes ){
				window.open('academico.php?modulo=relatorio/popupProfessoresEquivalentes&acao=C&printMode=t&ano='+ano+'&mes='+mes+'&portaria='+portaria,'','width=950,height=550');
			}else{
				alert("Selecione os parametros para impressão");
				document.location.href = 'academico.php?modulo=relatorio/professoresEquivalentes&acao=C';
		
			}
		});
		
		$('#btnGerarXls').click(function(){
			var ano = $('#ano').val();
			var mes = $('#mes').val();
			var portaria = $('#portaria').val();
			document.location.href = 'academico.php?modulo=relatorio/professoresEquivalentes&acao=C&ano='+ano+'&mes='+mes+'&portaria='+portaria+'&requisicao=gerarXls';
		});

		$('#btnPesquisar').click(function(){
			if($('[name=ano]').val() == '' || $('[name=mes]').val() == '' || $('[name=portaria]').val() == ''){
				alert('Os campos Ano, Mês e Portaria são obrigatórios!');
				return false;
			}
			$('#formulario').submit();
		});

		$('#btnLimpar').click(function(){
			document.location.href = 'academico.php?modulo=relatorio/professoresEquivalentes&acao=C';
		});
		
		$('[name=ano], [name=mes]').change(function(){

			ano = $('[name=ano]').val();
			mes = $('[name=mes]').val();			

			if(ano != '' && mes != ''){
				$.ajax({
					url		: 'academico.php?modulo=relatorio/professoresEquivalentes&acao=C',
					type	: 'post',
					data	: 'requisicao=carregaComboPortariasRel&ano='+ano+'&mes='+mes,
					success	: function(e){
						$('#td_portaria').html(e);
					}
				});
			}

			$('#divListaUniversidades').hide();

		});
		
	});
--></script>

<div id="div_relatorio_professores_equivalentes">
	<?php 
	$linha1 = "Banco de Professor Equivalente";
	$linha2 = "Relatório";

	monta_titulo($linha1, $linha2);
	?>
	<form name="formulario" id="formulario" method="post" action="">
		<input type="hidden" name="boPopup" id="boPopup" value="f" /> 
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtituloDireita notprint" width="160">Ano</td>
			<td>
				<?php 
				$arDados = array(
					array('codigo'=>'2008', 'descricao'=>'2008'),
					array('codigo'=>'2009', 'descricao'=>'2009'),
					array('codigo'=>'2010', 'descricao'=>'2010'),
					array('codigo'=>'2011', 'descricao'=>'2011'),
					array('codigo'=>'2012', 'descricao'=>'2012'),
					array('codigo'=>'2013', 'descricao'=>'2013'),
					array('codigo'=>'2014', 'descricao'=>'2014'),
					array('codigo'=>'2015', 'descricao'=>'2015'),
					array('codigo'=>'2016', 'descricao'=>'2016'),
					array('codigo'=>'2017', 'descricao'=>'2017'),
					array('codigo'=>'2018', 'descricao'=>'2018'),
					array('codigo'=>'2019', 'descricao'=>'2019'),
					array('codigo'=>'2020', 'descricao'=>'2020')
				);

				$db->monta_combo('ano', $arDados, 'S', 'Selecione...', '', '', '', '', 'S', 'ano');			
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Mês</td>
			<td id="td_mes">
				<?php 
				$arDados = array(
					array('codigo'=>'1', 'descricao'=>'Janeiro'),
					array('codigo'=>'2', 'descricao'=>'Fevereiro'),
					array('codigo'=>'3', 'descricao'=>'Março'),
					array('codigo'=>'4', 'descricao'=>'Abril'),
					array('codigo'=>'5', 'descricao'=>'Maio'),
					array('codigo'=>'6', 'descricao'=>'Junho'),
					array('codigo'=>'7', 'descricao'=>'Julho'),
					array('codigo'=>'8', 'descricao'=>'Agosto'),
					array('codigo'=>'9', 'descricao'=>'Setembro'),
					array('codigo'=>'10', 'descricao'=>'Outubro'),
					array('codigo'=>'11', 'descricao'=>'Novembro'),
					array('codigo'=>'12', 'descricao'=>'Dezembro')
				);
				$db->monta_combo('mes', $arDados, 'S', 'Selecione...', '', '', '', '', 'S', 'mes');			
				?>
			</td>			
		</tr>
		<tr>
			<td class="subtituloDireita">Portaria</td>
			<td id="td_portaria">
				<select name="portaria" class="CampoEstilo">
					<option value="">Selecione...</option>
				</select>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
				<td class="subtituloDireita">&nbsp;</td>
				<td class="subtituloEsquerda">
					<input type="button" value="Pesquisar" id="btnPesquisar" />
					<input type="button" value="Limpar" id="btnLimpar" />
				</td>
			</tr>
	</table>
	</form>
	<?php
	if($_POST && $_POST['portaria'] ){
		if($_POST['portaria']){
		$desc_portaria = montarPortaria($_POST['portaria']);
		}


        if(verificaTipoPesquisa($_GET) == 'A'){
            $cabecalho = array('Sigla', 'Órgão', 'UF','Nº Portaria', 'Ano', 'Mês', '20h', '40h', 'DE', 'Subst.', 'Visit.', 'Vagos', 'Total', 'BancoPEqv(ocupado)', 'Portaria MEC BPEQ', 'Saldo');
            $sql = montaSqlProfEquivalente($_POST);
        } else {
            $cabecalho = array('Sigla', 'Órgão', 'UF','Nº Portaria', 'Ano', 'Mês', 'FF 20h', 'FF 40h', 'DE', 'Sub. 20h' , 'Sub. 40h', 'Visit.', 'Vagos', 'Total', 'BancoPEqv(ocupado)', 'Portaria MEC BPEQ', 'Saldo');
            $sql = montaSqlProfEquivalenteNovo($_POST);
        }

//		$sql = montaSqlProfEquivalente();
//		ver($_POST['portaria']);
		$cabecalho = array('Código', 'Órgão', 'Município', 'UF', '20h', '40h', 'DE', 'Total', 'Subst.', 'Visit.', 'Vagos', 'BancoPEqv', $desc_portaria, 'Saldo');
		$db->monta_lista($sql, $cabecalho, 1000000, 10, 'N', '', '', '', '');
	}	
	?>
</div>
<?php if($_POST): ?>
	<center>
		<p>
		<input type="button" value="Imprimir" id="btnImprimir" onclick="valorPopup();" />
		<input type="button" value="Gerar Excel" id="btnGerarXls" />
		</p>
	</center>
<?php endif; ?>
<?php if($_POST['portaria']): ?>
	<script>
		$(function(){
			$.ajax({
				url		: 'academico.php?modulo=relatorio/professoresEquivalentes&acao=C',
				type	: 'post',
				data	: 'requisicao=carregaComboPortariasRel&ano='+<?=$_POST['ano']?>+'&mes='+<?=$_POST['mes']?>,
				success	: function(e){
					$('#td_portaria').html(e);
					$('[name=portaria]').val(<?=$_POST['portaria']?>);
				}
			});
		});
	</script>
<?php endif; ?>
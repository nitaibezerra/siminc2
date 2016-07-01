<?php 

if($_REQUEST['requisicao'] == 'mostraCamboPortaria'){
	
	$mes = trim($_REQUEST['mpemes']);
	$ano = trim($_REQUEST['mpeano']);
	
	if($ano && $mes){
	
		$semestre = ($_REQUEST['mpemes'] > 0 && $_REQUEST['mpemes'] < 7) ? 1 : 2;
	
		$sql = "select 
					ppe.ppeid as codigo, 
					ppe.ppenumero as descricao 
				from 
					academico.portariaprofequival ppe
				inner join 
					academico.portariavalor pov on pov.ppeid = ppe.ppeid
				where 
					ppeano = {$ano}
				and
					ppesemestre = {$semestre}
				and
					entid = {$_SESSION['academico']['entid']}
					
				";
		
		$db->monta_combo('ppeid', $sql, $habil, 'Selecione...', '', '', '', '');
	}
	die;
				
}

if($_REQUEST['requisicao'] == 'salvarProfessorEquivalente'){
	
	$mpeid 				= $_REQUEST['mpeid'];
	$ppeid 				= $_REQUEST['ppeid'] ? $_REQUEST['ppeid'] : 'null';
	$entid 				= $_REQUEST['entid'] ? $_REQUEST['entid'] : 'null';
	$mpeano 			= $_REQUEST['mpeano'] ? $_REQUEST['mpeano'] : 'null';
	$mpemes 			= $_REQUEST['mpemes'] ? $_REQUEST['mpemes'] : 'null';
	$mpevlr20h 			= $_REQUEST['mpevlr20h'] ? $_REQUEST['mpevlr20h'] : 'null';
	$mpevlr40h 			= $_REQUEST['mpevlr40h'] ? $_REQUEST['mpevlr40h'] : 'null';
	$mpevlrdedexclusiva = $_REQUEST['mpevlrdedexclusiva'] ? $_REQUEST['mpevlrdedexclusiva'] : 'null';
	$mpevlrsubstituto 	= $_REQUEST['mpevlrsubstituto'] ? $_REQUEST['mpevlrsubstituto'] : 'null';
	$mpevlrvisitante 	= $_REQUEST['mpevlrvisitante'] ? $_REQUEST['mpevlrvisitante'] : 'null';
	$mpevlrvagos 		= $_REQUEST['mpevlrvagos'] ? $_REQUEST['mpevlrvagos'] : 'null';
	$mpevlrbcequiv 		= $_REQUEST['mpevlrbcequiv'] ? $_REQUEST['mpevlrbcequiv'] : 'null';
	$mpevlrsaldo 		= $_REQUEST['mpevlrsaldo'] ? $_REQUEST['mpevlrsaldo'] : 'null';
	
	if(!$mpeid){
	
		$sql = "insert into academico.movprofequivalente
					(ppeid,entid,mpeano,mpemes,mpevlr20h,mpevlr40h,mpevlrdedexclusiva,mpevlrsubstituto,mpevlrvisitante,mpevlrvagos,mpevlrbcequiv,mpevlrsaldo,mpestatus)
				values
					({$ppeid},{$entid},{$mpeano},{$mpemes},{$mpevlr20h},{$mpevlr40h},{$mpevlrdedexclusiva},{$mpevlrsubstituto},{$mpevlrvisitante},{$mpevlrvagos},
					{$mpevlrbcequiv},{$mpevlrsaldo},'A')";
	}else{
		
		$sql = "update academico.movprofequivalente set
					ppeid					= {$ppeid},
					entid					= {$entid},
					mpeano					= {$mpeano},
					mpemes					= {$mpemes},
					mpevlr20h				= {$mpevlr20h},
					mpevlr40h				= {$mpevlr40h},
					mpevlrdedexclusiva		= {$mpevlrdedexclusiva},
					mpevlrsubstituto		= {$mpevlrsubstituto},
					mpevlrvisitante			= {$mpevlrvisitante},
					mpevlrvagos				= {$mpevlrvagos},
					mpevlrbcequiv			= {$mpevlrbcequiv},
					mpevlrsaldo				= {$mpevlrsaldo}					
				where mpeid = {$mpeid}";
	}
	
	$db->executar($sql);
	
	if($db->commit()){
		$db->sucesso('principal/professoresEquivalentes', '&ano='.$mpeano);
	}
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$mpeano = $_REQUEST['ano'] ? $_REQUEST['ano'] : date('Y');
$entid  = $_SESSION['academico']['entid'];
$orgid  = $_SESSION['academico']['orgid'];

$abacod_tela = 57135;
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( "Professores Equivalentes", "");

// cria o cabeçalho padrão do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();
echo $autoriazacaoconcursos->cabecalho_entidade($entid);

$habil = 'S';

if($mpeano && $entid){
	
	$sql = " select 	
				mpeid,
				ppeid,
				entid,
				mpeano,
				mpemes,
				mpevlr20h,
				mpevlr40h,
				mpevlrdedexclusiva,
				mpevlrsubstituto,
				mpevlrvisitante,
				mpevlrvagos,
				mpevlrbcequiv,
				mpevlrsaldo
			from 
				academico.movprofequivalente 
			where 
				entid = {$entid} and mpeano = {$mpeano}
			and 
				mpestatus = 'A'";
	
	$rs = $db->pegaLinha($sql);	
	if($rs) extract($rs);
	
	$ptvvalor = $db->pegaUm("select ptvvalor from academico.portariavalor where entid = {$entid}");
}
$total = $mpevlr20h+$mpevlr40h+$mpevlrdedexclusiva;
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js" type="text/javascript"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<link rel="stylesheet" type="text/css" href="../par/css/subacao.css"/>
<script>
	$(function(){

		$('#btnSalvar').click(function(){
			$('input').attr('disabled', false);			
			$('[name=requisicao]').val('salvarProfessorEquivalente');
			$('#formulario').submit();
		});

		$('#btnVoltar').click(function(){
			alert('Volta');
		});

		$('[name=mpevlr20h], [name=mpevlr40h], [name=mpevlrdedexclusiva]').change(function(){
			
			var mpevlr20h = $('[name=mpevlr20h]').val() ? $('[name=mpevlr20h]').val() : 0;
			var mpevlr40h = $('[name=mpevlr40h]').val() ? $('[name=mpevlr40h]').val() : 0;
			var mpevlrdedexclusiva = $('[name=mpevlrdedexclusiva]').val() ? $('[name=mpevlrdedexclusiva]').val() : 0;

			var total = parseInt(mpevlr20h)+parseInt(mpevlr40h)+parseInt(mpevlrdedexclusiva);
			
			if(total){
				$('[name=total]').attr('disabled', false);
				$('[name=total]').val(total);
				$('[name=total]').attr('disabled', true);
			}
			
		});

		$('[name=mpevlrsubstituto], [name=mpevlrvisitante], [name=mpevlrvagos], [name=mpevlrbcequiv]').change(function(){
			
			ptvvalor = $('[name=ptvvalor]').val();
			ptvvalor = ptvvalor ? ptvvalor : 0;

			mpevlr20h = $('[name=mpevlr20h]').val();
			mpevlr20h = mpevlr20h ? mpevlr20h : 0;

			mpevlr40h = $('[name=mpevlr40h]').val();
			mpevlr40h = mpevlr40h ? mpevlr40h : 0;

			mpevlrdedexclusiva = $('[name=mpevlrdedexclusiva]').val();
			mpevlrdedexclusiva = mpevlrdedexclusiva ? mpevlrdedexclusiva : 0;

			mpevlrsubstituto = $('[name=mpevlrsubstituto]').val();
			mpevlrsubstituto = mpevlrsubstituto ? mpevlrsubstituto : 0;

			mpevlrvisitante = $('[name=mpevlrvisitante]').val();
			mpevlrvisitante = mpevlrvisitante ? mpevlrvisitante : 0;

			// Calculo
			mpevlrbcequiv = (parseInt(mpevlr20h)*0.58) + (parseInt(mpevlr40h)*1) + (parseInt(mpevlrdedexclusiva)*1.7) + (parseInt(mpevlrsubstituto)*1) + (parseInt(mpevlrvisitante)*1.7);
			$('[name=mpevlrbcequiv]').val(mpevlrbcequiv);

			if(mpevlrbcequiv){

				mpevlrsaldo = parseInt(ptvvalor)-parseInt(mpevlrbcequiv);				
				$('[name=mpevlrsaldo]').val(mpevlrsaldo);
			}
			
		});
		

		var cosano = $('[name=mpeano]').val(); 
		
		$('#abaAnos' + cosano).attr("style","display:table-row");
		$('#aba_' + cosano).attr("style","width:50px;background-position:0% -150px;color: #333;");
		$('#aba_' + cosano + ' a:first').attr("style","background-position:100% -150px;color: #333;");

		$('[name=mpemes]').change(function(){

			var ano = $('[name=mpeano]').val();

			$('#combo_portarias').html('Carregando...');
			
			$.ajax({
				url		: 'academico.php?modulo=principal/professoresEquivalentes&acao=A',
				type	: 'post',
				data	: 'requisicao=mostraCamboPortaria&mpeano='+ano+'&mpemes='+this.value,
				success	: function(e){
					$('#combo_portarias').html(e);
				}
			});
		});

	});
</script>

<style>
	#tabs{
		width: 95%;		
	}
	.ui-tabs-nav{
		background: #dcdcdc;
		border: 0px solid black;
	}
</style>

<table align="center" border="0" class="tabela" cellpadding="0" cellspacing="0" id="tabAbasSelecaoCosano" style="margin-top:5px;">
	<tr id="abasSelecaoCosano">
		<?php		
		$arAnos = array(
			array('codigo' => '2008', 'descricao' => '2008'),
			array('codigo' => '2009', 'descricao' => '2009'),
			array('codigo' => '2010', 'descricao' => '2010'),
			array('codigo' => '2011', 'descricao' => '2011'),
			array('codigo' => '2012', 'descricao' => '2012'),
			array('codigo' => '2013', 'descricao' => '2013'),
			array('codigo' => '2014', 'descricao' => '2014'),
			array('codigo' => '2015', 'descricao' => '2015'),
			array('codigo' => '2016', 'descricao' => '2016'),
			array('codigo' => '2017', 'descricao' => '2017'),
			array('codigo' => '2018', 'descricao' => '2018'),
			array('codigo' => '2019', 'descricao' => '2019'),
			array('codigo' => '2020', 'descricao' => '2020')
		);
		foreach( $arAnos as $ano ){
			echo "<td id=\"aba_{$ano['codigo']}\" style=\"width: 50px;\" ><a href=\"academico.php?modulo=principal/professoresEquivalentes&acao=A&ano={$ano['codigo']}\">{$ano['codigo']}</a></td>";
		}
		?>		
	</tr>	
</table>

<form name="formulario" id="formulario" method="post" action="">
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="mpeano" value="<?php echo $mpeano; ?>" />
	<input type="hidden" name="mpeid" value="<?php echo $mpeid ? $mpeid : ''; ?>" />	
	<input type="hidden" name="entid" value="<?php echo $entid; ?>" />
	<input type="hidden" name="ptvvalor" value="<?php echo $ptvvalor; ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan="2" class="subtituloCentro">Lançamentos</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="120">Mês de Referência</td>
			<td>
				<?php 
				$sql = "select mescod as codigo, mesdsc as descricao from public.meses";
				$db->monta_combo('mpemes', $sql, $habil, 'Selecione...', '', '', '', '');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="120">Portaria</td>
			<td id="combo_portarias">
				
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="120">20 h</td>
			<td><?php echo campo_texto('mpevlr20h', 'S', $habil, '', 8, $max, $masc, ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">40 h</td>
			<td><?php echo campo_texto('mpevlr40h', 'S', $habil, '', 8, $max, $masc, ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Dedicação Exclusiva</td>
			<td><?php echo campo_texto('mpevlrdedexclusiva', 'S', $habil, '', 8, $max, $masc, ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Total</td>
			<td><?php echo campo_texto('total', 'S', 'N', '', 8, $max, $masc, ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Substituto</td>
			<td><?php echo campo_texto('mpevlrsubstituto', 'S', $habil, '', 8, $max, $masc, ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Visitante</td>
			<td><?php echo campo_texto('mpevlrvisitante', 'S', $habil, '', 8, $max, $masc, ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Vagos</td>
			<td><?php echo campo_texto('mpevlrvagos', 'S', $habil, '', 8, $max, $masc, ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">BancoPEqv</td>
			<td><?php echo campo_texto('mpevlrbcequiv', 'S', $habil, '', 8, $max, $masc, ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Saldo</td>
			<td><?php echo campo_texto('mpevlrsaldo', 'S', 'N', '', 8, $max, $masc, ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="SubtituloEsquerda">
				<input type="button" value="Salvar" id="btnSalvar" />
				<input type="button" value="Voltar" id="btnVoltar" />
			</td>
		</tr>	
	</table>
</form>
<?php if($ppeid): ?>
	<script>
		$(function(){
			
			var ano = $('[name=mpeano]').val();
			var mes = $('[name=mpemes]').val();
			var ppeid = '<?php echo $ppeid; ?>';

			$('#combo_portarias').html('Carregando...');
			
			$.ajax({
				url		: 'academico.php?modulo=principal/professoresEquivalentes&acao=A',
				type	: 'post',
				data	: 'requisicao=mostraCamboPortaria&mpeano='+ano+'&mpemes='+mes,
				success	: function(e){
					$('#combo_portarias').html(e);
					$('[name=ppeid]').val(ppeid);					
				}
			});
		});
	</script>
<?php endif; ?>
<?
//if( $_REQUEST['chkaluno'][0] ){

if( $_POST['arquivo'] > 0 && $_POST['ini'] > 0 && $_POST['fim'] >= 0 ){
// 	ver(d);
	ini_set("memory_limit", "2048M");
	set_time_limit(80000);

	$file = 'SIMEC_PROJOVEM_GERANISALUNOS_'.date("dmYHis").'.txt';
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. $file;
	$fp = fopen($caminho, "w");
	
	//inicio header
		$codcampo = array(0 => '0900',
						 1 => '0829',
						 2 => '0313',
						 3 => '0413',
						 4 => '0903',
						 5 => '0913'
					);
					 
		$valcampo = array(0 => str_pad('C', 212, " ", STR_PAD_RIGHT),
						 1 => str_pad('00394445003038', 212, " ", STR_PAD_RIGHT),
						 2 => str_pad('MINISTERIO DA EDUCACAO', 212, " ", STR_PAD_RIGHT),
						 3 => str_pad('O', 212, " ", STR_PAD_RIGHT),
						 4 => str_pad(date('dmY'), 212, " ", STR_PAD_RIGHT),
						 5 => str_pad('0002', 212, " ", STR_PAD_RIGHT)
					 );
	
		$i=0;
		$seq = 0;
		$linha = 1;
		for($i; $i<6; $i++){
			
			$campo1 = str_pad($linha, 11, "0", STR_PAD_LEFT);
			$campo2 = '000000137438953472';
			//$campo3 = '';
			//$campo4 = '99';
			$campo3 = '';
			$campo4 = '00';
			$campo5 = str_pad($seq, 3, "0", STR_PAD_LEFT);
			$campo6 = $codcampo[$seq];
			$campo7 = '00';
			$campo8 = '00';
			$campo9 = $valcampo[$seq];
			$campo10 = '';
			$campo11 = '';
			$campo12 = '0000';
			
			$seq++;
			
			//imprime header
			echo $campo1.$campo2.$campo3.$campo4.$campo5.$campo6.$campo7.$campo8.$campo9.$campo10.$campo11.$campo12;
			
			//quebra linha
			if($i != 6) echo chr(13).chr(10);
			
			$linha++;
		}
	//fim header
	
	//inicio conteudo
	
		$sql = "select caecpf,caenome,to_char(caedatanasc, 'DDMMYYYY') as caedatanasc,
						   CASE when caenomemae is null then
						   			'IGNORADA'
						   		else
						   			caenomemae
						   END as caenomemae,
						   CASE when caenomepai is null or caenomepai = '' then
						   			'IGNORADO'
						   		else
						   			caenomepai
						   END as caenomepai,
						   CASE when caesexo is null or caesexo = '' then
						   			'M'
						   		else
						   			caesexo
						   END as caesexo,
						   CASE when caenaturalidade is null or caenaturalidade = '' or caenaturalidade = 'B' then
						   			'1'
						   		else
						   			'2'
						   END as caenaturalidade,
						   CASE when caecep is null or caecep = '' then
						   			'70089000'
						   		else
						   			caecep
						   END as caecep,
						   '1' as caetipoendereco,
						   'R  ' as caetipologradouro,
						   CASE when caelogradouro is null or caelogradouro = '' then
						   			'S N'
						   		else
						   			caelogradouro
						   END as caelogradouro,
						   'NUM  ' as caesigla,
						   CASE when caenumero is null or caenumero = '' then
						   			'S N'
						   		else
						   			caenumero
						   END as caeposicao,
						   CASE when caecomp is null or caecomp = '' then
						   			'S N'
						   		else
						   			caecomp
						   END as caecomp,
						   CASE when caebairro is null or caebairro = '' then
						   			'S N'
						   		else
						   			caebairro
						   END as caebairro,
						   CASE when muncod is null or muncod = '' then
						   			'5300108'
						   		else
						   			muncod
						   END as muncod  
					from projovemurbano.cadastroestudante 
					where (caenispispasep = '' or caenispispasep is null or caenispispasep = '0')
					AND ppuid = {$_SESSION['projovemurbano']['ppuid']}
					order by caeid
					LIMIT ".$_POST['ini']." OFFSET ".$_POST['fim']."
					";

		$dados2 = $db->carregar($sql);
		
		foreach($dados2 as $dados){
			/*
			$sql = "select caenome,to_char(caedatanasc, 'DDMMYYYY') as caedatanasc,
						   caenomemae,caenomepai,caesexo,caenaturalidade,
						   caecpf,
						   caecep,
						   '1' as caetipoendereco,
						   'R  ' as caetipologradouro,
						   caelogradouro,
						   'NUM. ' as caesigla,
						   caenumero as caeposicao,
						   caecomp,
						   caebairro,
						   muncod  
					from projovemurbano.cadastroestudante 
					where caeid = $v";
			$dados = $db->pegaLinha($sql);
			*/
			$codcampo = array(0 => '0902',
							 1 => '0195',
							 2 => '0197',
							 3 => '0200',
							 4 => '0199',
							 5 => '0201',
							 6 => '0386',
							 7 => '0386',
							 8 => '0370'
						);
							 
			$valcampo = array(0 => str_pad('I', 212, " ", STR_PAD_RIGHT),
							 1 => str_pad(subistituiCaracteres($dados['caenome']), 212, " ", STR_PAD_RIGHT),
							 2 => str_pad(subistituiCaracteres($dados['caedatanasc']), 212, " ", STR_PAD_RIGHT),
							 3 => str_pad(subistituiCaracteres($dados['caenomemae']), 212, " ", STR_PAD_RIGHT),
							 4 => str_pad(subistituiCaracteres($dados['caenomepai']), 212, " ", STR_PAD_RIGHT),
							 5 => str_pad(subistituiCaracteres($dados['caesexo']), 212, " ", STR_PAD_RIGHT),
							 6 => str_pad("0010", 212, " ", STR_PAD_RIGHT),
							 //7 => str_pad($dados['caenaturalidade'], 180, " ", STR_PAD_RIGHT),
							 7 => str_pad("1", 212, " ", STR_PAD_RIGHT),
							 8 => str_pad(subistituiCaracteres($dados['caecpf']), 212, " ", STR_PAD_RIGHT)
						 );
		
			//Dados do aluno
				$seq = 0;
				for($i=0; $i<9; $i++){
					
					$campo1 = str_pad($linha, 11, "0", STR_PAD_LEFT);
					$campo2 = '000000137438953472';
					$campo3 = '';
					$campo4 = '02';
					$campo5 = str_pad($seq, 3, "0", STR_PAD_LEFT);
					$campo6 = $codcampo[$seq];
					$campo7 = ($i==7 ? '01' : '00');
					$campo8 = '00';
					$campo9 = $valcampo[$seq];
					$campo10 = '';
					$campo11 = '';
					$campo12 = '0000';
					//$campo13 =  str_pad(0, 31, "0", STR_PAD_RIGHT);
					
// 					$campo1 = subistituiCaracteres($campo1);
// 					$campo2 = subistituiCaracteres($campo2);
// 					$campo3 = subistituiCaracteres($campo3);
// 					$campo4 = subistituiCaracteres($campo4);
// 					$campo5 = subistituiCaracteres($campo5);
// 					$campo6 = subistituiCaracteres($campo6);
// 					$campo7 = subistituiCaracteres($campo7);
// 					$campo8 = subistituiCaracteres($campo8);
// 					$campo9 = subistituiCaracteres($campo9);
// 					$campo10 = subistituiCaracteres($campo10);
// 					$campo11 = subistituiCaracteres($campo11);
// 					$campo12 = subistituiCaracteres($campo12);
					
					//imprime
					echo $campo1.$campo2.$campo3.$campo4.$campo5.$campo6.$campo7.$campo8.$campo9.$campo10.$campo11.$campo12.$campo13;
					
					$seq++;
	
					//quebra linha
					if($i != 9) echo chr(13).chr(10);
					
					$linha++;
				}
			//Fim Dados do aluno
		
						 	
				
			//Endereço do aluno
			$valend = array(0 => str_pad(subistituiCaracteres($dados['caecep']), 212, " ", STR_PAD_RIGHT),
							1 => str_pad(subistituiCaracteres($dados['caetipoendereco']), 212, " ", STR_PAD_RIGHT),
							2 => str_pad(subistituiCaracteres($dados['caetipologradouro']), 212, " ", STR_PAD_RIGHT),
							3 => str_pad(subistituiCaracteres($dados['caelogradouro']), 212, " ", STR_PAD_RIGHT),
							4 => str_pad(subistituiCaracteres($dados['caesigla']), 212, " ", STR_PAD_RIGHT),
							5 => str_pad(subistituiCaracteres($dados['caeposicao']), 212, " ", STR_PAD_RIGHT),
							6 => str_pad(subistituiCaracteres($dados['caecomp']), 212, " ", STR_PAD_RIGHT),
							7 => str_pad(subistituiCaracteres($dados['caebairro']), 212, " ", STR_PAD_RIGHT),
							8 => str_pad(subistituiCaracteres($dados['muncod']), 212, " ", STR_PAD_RIGHT)
						 );
						 
			//$seq = 0;
			$seqend=0;
			for($i=0; $i<9; $i++){
				
				$campo1 = str_pad($linha, 11, "0", STR_PAD_LEFT);
				$campo2 = '000000137438953472';
				$campo3 = '';
				$campo4 = '02';
				$campo5 = str_pad($seq, 3, "0", STR_PAD_LEFT);
				$campo6 = '0911'; //$codcampo[$seq];
				$campo7 = '0'.$seqend;
				$campo8 = '00';
				$campo9 = $valend[$seqend];
				$campo10 = '';
				$campo11 = '';
				$campo12 = '0000';
				//$campo13 =  str_pad(0, 31, "0", STR_PAD_RIGHT);
				
				$seqend++;
				$seq++;
				
// 				$campo1 = subistituiCaracteres($campo1);
// 				$campo2 = subistituiCaracteres($campo2);
// 				$campo3 = subistituiCaracteres($campo3);
// 				$campo4 = subistituiCaracteres($campo4);
// 				$campo5 = subistituiCaracteres($campo5);
// 				$campo6 = subistituiCaracteres($campo6);
// 				$campo7 = subistituiCaracteres($campo7);
// 				$campo8 = subistituiCaracteres($campo8);
// 				$campo9 = subistituiCaracteres($campo9);
// 				$campo10 = subistituiCaracteres($campo10);
// 				$campo11 = subistituiCaracteres($campo11);
// 				$campo12 = subistituiCaracteres($campo12);
				
				//imprime
				echo $campo1.$campo2.$campo3.$campo4.$campo5.$campo6.$campo7.$campo8.$campo9.$campo10.$campo11.$campo12.$campo13;
				
				//quebra linha
				if($i != 9) echo chr(13).chr(10);
				
				$linha++;
			}
			//Fim Endereço do aluno
			
		}
	//fim conteudo
	
		
	//inicio rodape
		$file = 'MIC.ISO.EPROJOVE.ED0003.D'.date("ymd").'.V0'.($_POST['arquivo']-1);
		
		$codcampo = array(0 => '0908',
						 1 => '0912'
					);
					 
		$valcampo = array(0 => str_pad($file, 212, " ", STR_PAD_RIGHT),
						 1 => str_pad(($linha+1), 212, " ", STR_PAD_RIGHT)
					 );
	
		$seq = 0;
		$i=0;
		for($i; $i<2; $i++){
			
			$campo1 = str_pad($linha, 11, "0", STR_PAD_LEFT);
			$campo2 = '000000137438953472';
			//$campo3 = '';
			//$campo4 = '00';
			$campo3 = '';
			$campo4 = '99';
			$campo5 = str_pad($seq, 3, "0", STR_PAD_LEFT);
			$campo6 = $codcampo[$seq];
			$campo7 = '00';
			$campo8 = '00';
			if($i==1){
				$campo9 = str_pad($linha, 9, "0", STR_PAD_LEFT);
				$campo9 = str_pad($campo9, 212, " ", STR_PAD_RIGHT);
			}
			else{
				$campo9 = $valcampo[$seq];
			}
			$campo10 = '';
			$campo11 = ($i==0 ? '' : '');
			$campo12 = '0000';
			//$campo13 =  str_pad(0, 31, "0", STR_PAD_RIGHT);
			
			$seq++;
// 			subistituiCaracteres;
// 			$campo1 = subistituiCaracteres($campo1);
// 			$campo2 = subistituiCaracteres($campo2);
// 			$campo3 = subistituiCaracteres($campo3);
// 			$campo4 = subistituiCaracteres($campo4);
// 			$campo5 = subistituiCaracteres($campo5);
// 			$campo6 = subistituiCaracteres($campo6);
// 			$campo7 = subistituiCaracteres($campo7);
// 			$campo8 = subistituiCaracteres($campo8);
// 			$campo9 = subistituiCaracteres($campo9);
// 			$campo10 = subistituiCaracteres($campo10);
// 			$campo11 = subistituiCaracteres($campo11);
// 			$campo12 = subistituiCaracteres($campo12);
// 			$campo13 = subistituiCaracteres($campo13);
			//imprime rodape
			echo $campo1.$campo2.$campo3.$campo4.$campo5.$campo6.$campo7.$campo8.$campo9.$campo10.$campo11.$campo12.$campo13;
			
			//quebra linha
			if($i == 0) echo chr(13).chr(10);
			
			$linha++;
		}
	//fim inicio rodape
	
	header("Content-Type:text/plain");
	header("Content-Disposition:attachment; filename=".$file.".txt");
	header("Content-Transfer-Encoding:binary");
	die;
	
}

// $nome1 = 'MAhttp://simec.mec.gov.br/imagens/obrig.gifRIA DA CRUZ DA CONCEICAO';
// $nome = subistituiCaracteres($nome1);
// ver($nome1,$nome,d);
checkAno();

include  APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ.'projovemurbano/modulos/principal/filtroAnoExercicio.inc';
echo '<br>';

$menu = array(0 => array("id" => 1, "descricao" => "Gerar Arquivo CEF", 			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/gerenciarNIS&acao=A"),
				  1 => array("id" => 2, "descricao" => "Carregar Arquivo NIS", 	"link" => "/projovemurbano/projovemurbano.php?modulo=principal/carregarNIS&acao=A")
			  	  );

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

monta_titulo('Projovem Urbano', 'Gerenciar NIS. Selecione os filtros abaixo.');

$bloq = 'S';
$disabled = '';
// $teste1 = 'áéãô.,@!$¨¨¨&()_';
// $teste = subistituiCaracteres($teste1);
// ver($teste1,$teste,d);
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/projovemurbano.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script language="javascript" type="text/javascript">

jQuery(document).ready(function()
{

	jQuery('[id="chktodos"]').click(function(){
		if(jQuery(this).attr('checked') == true){
			jQuery('[id="chkaluno"]').attr('checked',true);
		}else{
			jQuery('[id="chkaluno"]').attr('checked',false);
		}
	});
	
	jQuery('[name="caehistorico"]').click(function(){
		if(jQuery(this).val()=='TRUE'){
			jQuery('[name="caetestepro"][value="FALSE"]').attr('checked',true);
		}else{
			jQuery('[name="caetestepro"][value="TRUE"]').attr('checked',true);
		}
	});
	jQuery('[name="caetestepro"]').click(function(){
		if(jQuery(this).val()=='TRUE'){
			jQuery('[name="caehistorico"][value="FALSE"]').attr('checked',true);
		}else{
			jQuery('[name="caehistorico"][value="TRUE"]').attr('checked',true);
		}
	});
	jQuery('[name="nomesocial"]').click(function(){
		if( jQuery(this).val() == 'S' ){
			jQuery('#tr_nomesocial').show();
		}else{
			jQuery('#tr_nomesocial').hide();
			jQuery('#caenomesocial').val('');
		}
	});
});

function carregarMunicipios2(estuf) {
	var pjuesfera = jQuery('#pjuesfera').val();
	estuf = jQuery('#estuf').val();
	if( pjuesfera == 'M' && estuf != '' ){
		jQuery('#tr_muncod').show();
		jQuery('#td_muncod').html('Carregando...');
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location,
	   		data: "req=carregarMunicipios2&estuf="+estuf,
	   		async: false,
	   		success: function(msg){jQuery('#td_muncod').html(msg);}
	 	});
	}else if( pjuesfera == 'E'){
		jQuery('#tr_muncod').hide();
		jQuery('#td_muncod').html('');
		if( estuf != '' ){
			carregarPolo(estuf);
		}
	}else if( estuf != '' ){
		alert('Escolha uma esfera.');
	}
}

function carregarPolo(cod) {
	var pjuesfera = jQuery('#pjuesfera').val();
	if( pjuesfera == 'M' ){ cod = '&muncod='+jQuery('#muncod').val() }else{ cod = '&estuf='+jQuery('#estuf').val() }
	jQuery.ajax({
   		type: "POST",
   		url: window.location,
   		data: "req=testaPolo"+cod,
   		async: false,
   		success: function(msg){
   			if( msg == 'S' ){
   				jQuery.ajax({
   			   		type: "POST",
   			   		url: window.location,
   			   		data: "req=carregarPolo"+cod,
   			   		async: false,
   			   		success: function(msg){
   			   			jQuery('#td_polo').html(msg);
   			   			jQuery('#tr_polo').show();
   			   			jQuery('#td_nucleo').html('<select id="endestuf" disabled="disabled" style="width: auto" class="CampoEstilo" name="endestuf_disable"><option value="">Selecione</option></select>');
   			   		}
   			 	});
   	   		}else{
		   		jQuery('#tr_polo').hide();
			   	jQuery('#td_polo').html('');
				jQuery('#td_nucleo').html('Carregando...');
				jQuery.ajax({
			   		type: "POST",
			   		url: window.location,
			   		data: "req=buscarNucleosSemId"+cod+"&bloq=<?=$bloq ?>",
			   		async: false,
			   		success: function(msg){
				   		jQuery('#td_nucleo').html(msg);
				   	}
			 	});
   	   	   	}
		}
 	});
}

function buscarNucleos(polid, bloq) {
	if( polid != '' ){
		jQuery('td_nucleo').html('Carregando...');
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location,
	   		data: "req=buscarNucleos&polid="+polid+"&bloq=<?=$bloq ?>",
	   		async: false,
	   		success: function(msg){document.getElementById('td_nucleo').innerHTML = msg;}
	 	});
	}
}

function buscarTurmas(nucid, bloq) {
	if( nucid != '' ){
		document.getElementById('td_turma').innerHTML = 'Carregando...';
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location,
	   		data: "req=buscarTurmas&nucid="+nucid+"&bloq=<?=$bloq ?>",
	   		async: false,
	   		success: function(msg){document.getElementById('td_turma').innerHTML = msg;}
	 		});
	}
}

function geraArquivo(arq, ini, fim){
	document.getElementById('arquivo').value = arq;
	document.getElementById('ini').value = ini;
	document.getElementById('fim').value = fim;
	document.getElementById('form_busca').submit();
}

/*
function geraQtd(qtd){
	location.href="projovemurbano.php?modulo=principal/gerenciarNIS&acao=A&qtd="+qtd;
}
*/ 

</script>
<form id="form_busca" name="form_busca" method="POST">
	
	<input type="hidden" name="arquivo" id="arquivo" value="">
	<input type="hidden" name="ini" id="ini" value="">
	<input type="hidden" name="fim" id="fim" value="">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2" width="5%">
				<b>Geração de arquivos</b>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Total de Alunos:</td>
			<td>
				<? 
				$sql = "select count(caeid) as total  
					from projovemurbano.cadastroestudante 
					where (caenispispasep = '' or caenispispasep is null)
					AND ppuid = {$_SESSION['projovemurbano']['ppuid']}";
				$total = $db->pegaUm($sql);
				echo $total;
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Quantidade de Arquivos:</td>
			<td>
				<? 
				$ini = (int) 1;
				$fim = (int) 100;
				
				$qtdArray = array();
				for($i=$ini; $i<=$fim; $i++){
					array_push( $qtdArray, array("codigo"=>$i, "descricao"=>$i) );
				}
				
				$qtd = $_REQUEST['qtd'];
				
				$db->monta_combo( 'qtd', $qtdArray, 'S', '--','', '' );
				
				echo '&nbsp;&nbsp;<input type="button" name="btngera" value="Gerar Arquivos" onclick="geraArquivo(0,0,0);">'; 
				?>
			</td>
		</tr>
		<?if($_REQUEST['qtd']){?>
			<tr>
				<td class="SubTituloDireita" colspan="2">&nbsp;</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Baixar Arquivos:</td>
				<td>
					<? 
					$ini = (int) 1;
					$fim = (int) $_REQUEST['qtd'];
					
					$reg = ($total/$_REQUEST['qtd']);
					if (!is_int($reg)) $reg = intval($reg)+1;
					
					$qtdArray = array();
					for($i=$ini; $i<=$fim; $i++){
						echo '<input type="button" name="btngera'.$i.'" value="Arquivo '.$i.'" onclick="geraArquivo('.$i.','.$reg.','.($i>1 ? ($reg*$i)-$reg : 0).');"><br>';
					}
					?>
				</td>
			</tr>
		<?}?>
	</table>
</form>
<!--
<form id="form_busca" name="form_busca" method="POST">
	 
	<input type="hidden" name="req" value="">
	<input type="hidden" name="estuf" value="<?=$_SESSION['projovemurbano']['estuf'] ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2" width="30%">
				<b>Filtros de pesquisa</b>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Esfera</td>
			<td>
				<? 
				$sql = "SELECT DISTINCT 
							CASE 
								WHEN estuf IS NULL  THEN 'M' 
								WHEN muncod IS NULL THEN 'E' 
							END as codigo, 
							CASE 
								WHEN estuf IS NULL  THEN 'Municipal' 
								WHEN muncod IS NULL THEN 'Estadual' 
							END as descricao
						FROM 
							projovemurbano.projovemurbano";
				$db->monta_combo('pjuesfera', $sql, $bloq, 'Selecione', 'carregarMunicipios2', '', '', '', 'S', 'pjuesfera');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UF</td>
			<td>
				<? 
				$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
				$db->monta_combo('estuf', $sql, $bloq, 'Selecione', 'carregarMunicipios2', '', '', '', 'S', 'estuf');
				?>
			</td>
		</tr>
		<tr id="tr_muncod">
			<td class="SubTituloDireita">Munícipio</td>
			<td id="td_muncod">
				<? $db->monta_combo('endmuncod', array(), $bloq, 'Selecione', '', '', '', '', 'S', 'endmuncod'); ?>
			</td>
		</tr>
		<tr id="tr_polo" style="display:none">
			<td class="SubTituloDireita"><b>Polo</b></td>
			<td id="td_polo">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Núcleo / Escola</b></td>
			<td id="td_nucleo"><? $db->monta_combo('nucid', array(), 'N', 'Selecione', '', '', '', '', 'N', 'nucid'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Turma</b></td>
			<td id="td_turma"><? $db->monta_combo('endestuf', array(), 'N', 'Selecione', '', '', '', '', 'N', 'endestuf'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Matricula:</td>
			<td><? echo campo_texto('matricula', "N", "S", "Matricula", 12, 8, "########", "", '', '', 0, 'id="matricula"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">CPF:</td>
			<?php $caecpf = $_POST['caecpf']?>
			<td><? echo campo_texto('caecpf', "N", "S", "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="caecpf"', '', '', '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome:</td>
			<td><? echo campo_texto('caenome', "N", "S", "Nome", 50, 255, "", "", '', '', 0, 'id="caenome"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Status:</td>
			<td>
				<input type="radio" name="status" value="A" <?=($_POST['status']=='A'?'checked':'') ?>/> Ativo
				<input type="radio" name="status" value="I" <?=($_POST['status']=='I'?'checked':'') ?>/> Inativo - outros
				<input type="radio" name="status" value="D" <?=($_POST['status']=='D'?'checked':'') ?>/> Inativo - desistência
				<input type="radio" name="status" value=" " <?=($_POST['status']==' '?'checked':'') ?>/> Todos
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<input type="button" name="pesquisar" value="Pesquisar" onclick="document.getElementById('form_busca').submit();">
			</td>
		</tr>
	</table>
	 
</form>
-->

<?

$where = Array();

if( $_POST['pjuesfera'] == 'E' ){
	if( $_POST['estuf'] != '' ){
		$where[] = "(pju.estuf = '".$_POST['estuf']."')";
	}else{
		$where[] = "(pju.estuf IS NOT NULL)";
	}
}
if( $_POST['pjuesfera'] == 'M' ){
	if( $_POST['muncod'] != '' ){
		$where[] = "(pju.muncod = '".$_POST['muncod']."')";
	}else{
		$where[] = "(pju.muncod IS NOT NULL)";
	}
}
if( $_POST['status'] != '' && $_POST['status'] != ' ' ){
	$where[] = "cae.caestatus = '".$_POST['status']."'";
}
if( $_POST['justificativaD'] ){
	$where[] = "cae.caejustificativainativacao = 'DESISTENTE'";
}
if( $_POST['justificativaF'] ){
	$where[] = "cae.caejustificativainativacao = 'FREQUENCIA INSUFICIENTE'";
}
if( $_POST['justificativaO'] ){
	$where[] = "cae.caejustificativainativacao NOT IN ('DESISTENTE','FREQUENCIA INSUFICIENTE')";
}
if( $_POST['polid'] ){
	$where[] = "cae.polid = ".$_POST['polid'];
}
if( $_POST['nucid'] || $_POST['nucid_disable'] ){
	$_POST['nucid'] = $_POST['nucid'] ? $_POST['nucid'] : $_POST['nucid_disable'];
	$where[] = "cae.nucid = ".$_POST['nucid'];
}
if( $_POST['turid'] ){
	$where[] = "cae.turid = ".$_POST['turid'];
}
if( $_POST['matricula'] ){
	$where[] = "caeano||lpad(cae.caeid::varchar,6,'0') ilike '%".$_POST['matricula']."%'";
}
if( $_POST['caenome'] ){
	$where[] = "UPPER(cae.caenome) ilike '%'||UPPER('".$_POST['caenome']."')||'%'";
}
if( $_POST['caecpf'] ){
	$where[] = "cae.caecpf = '".str_replace(Array('.','-'),'',$_POST['caecpf'])."'";
}

if($_SESSION['projovemurbano']['ppuano'] == 2013){
		$ano = 2013;
}

//$acoes = "<input type=\"checkbox\" name=\"chkaluno[]\" id=\"chkaluno\" value=\"'||cae.caeid||'\">";
$acoes = " - ";

if(!$db->testa_superuser()) {
	
        $perfis = pegaPerfilGeral();
	
	
	if(in_array(PFL_SECRETARIO_MUNICIPAL, $perfis)) {
		$inner_municipio = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.muncod=ende.muncod) AND rpustatus='A'";
		$acoes = '';
	}

	if(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
		$inner_municipio = "INNER JOIN projovemurbano.coordenadorresponsavel cr ON pju.pjuid=cr.pjuid AND cr.corcpf='".$_SESSION['usucpf']."'";
		$acoes = '';
	}
	
	if(in_array(PFL_SECRETARIO_ESTADUAL, $perfis)) {
		$inner_estado = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.estuf=ende.estuf)";
		$acoes = '';
	}
	
	if(in_array(PFL_COORDENADOR_ESTADUAL, $perfis)) {
		$inner_estado = "INNER JOIN projovemurbano.coordenadorresponsavel cr ON pju.pjuid=cr.pjuid AND cr.corcpf='".$_SESSION['usucpf']."'";
		$acoes = '';
	}
	
	if(in_array(PFL_DIRETOR_POLO, $perfis)) {
		$inner_estado = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.polid=cae.polid)";
		$acoes = '';
	}
	
	if(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
		$inner_estado = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.entid=nes.entid)";
		$acoes = '';
	}
	
	if(in_array(PFL_CONSULTA, $perfis)) {
		$acoes = '';
	}
}


//$acoes2 = "";

$sql = "SELECT DISTINCT
			'<center>$acoes</center>' as acoes,
			caenome,
			caecpf,
			caeano||lpad(cae.caeid::varchar,6,'0') as ninscricao, 
			CASE WHEN pju.estuf is not null THEN 
					'ESTADUAL: '||upper(est.estdescricao) 
				 ELSE 'MUNICIPAL: '||upper(mun.mundescricao)||' - '||mun.estuf 
			END as coord,
			'PÓLO&nbsp;'||polid as polo,
			'NÚCLEO '||nuc.nucid||
			CASE WHEN nuetipo = 'S' THEN 
					', SEDE: ' 
				 ELSE ', ANEXO:' 
			END||ent.entnome as entnome,
			turdesc 
		FROM 
			projovemurbano.cadastroestudante cae
		INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid  = cae.pjuid
		LEFT  JOIN projovemurbano.turma          tur ON tur.turid  = cae.turid
		LEFT  JOIN projovemurbano.nucleo         nuc ON nuc.nucid  = tur.nucid
		LEFT  JOIN projovemurbano.nucleoescola   nes ON nes.nucid  = nuc.nucid AND nes.entid = tur.entid
		LEFT  JOIN entidade.entidade 	         ent ON ent.entid  = tur.entid
		LEFT  JOIN entidade.entidadeendereco     etd ON etd.entid  = ent.entid
		LEFT  JOIN entidade.endereco 	        ende ON ende.endid = etd.endid
		LEFT  JOIN territorios.municipio         mun ON mun.muncod = pju.muncod
		LEFT  JOIN territorios.estado            est ON est.estuf  = pju.estuf
		{$inner_polo_filtro}
		$inner_estado
		$inner_municipio
		WHERE
			(caenispispasep = '' or caenispispasep is null)
		AND cae.ppuid = {$_SESSION['projovemurbano']['ppuid']}
			".(($filtro_nucleo)?" AND nuc.nucid IN('".implode("','",$filtro_nucleo)."')":"")
			.(($where)?" AND ".implode(" AND ",$where) :"");
//ver($sql);
$cabecalho = array(" - ","Aluno","CPF","Matrícula","Coordenação","Pólo","Núcleo","Turma");

//echo '<table width="95%" align="center"><tr><td>';
//echo '<input type="checkbox" name="chktodos" id="chktodos"> <b>Selecionar Todos</b>';
//echo '&nbsp;&nbsp;&nbsp;&nbsp;';
//echo '<input type="button" name="btngera" value="Gerar Arquivo" onclick="geraArquivo();">';
//echo '</td></tr></table>';



$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', $par2, 'formnis');

//echo '<table width="95%" align="center"><tr><td>';
//echo '<input type="button" name="btngera" value="Gerar Arquivo" onclick="geraArquivo();">';
//echo '</td></tr></table>';
?>
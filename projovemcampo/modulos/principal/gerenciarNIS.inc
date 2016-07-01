<?

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
	
		$sql = "select 
					estcpf,estnome,
					to_char(estdatanascimento, 'DDMMYYYY') as estdatanascimento,
					   CASE when estnomemae is null then
								'IGNORADA'
							else
								estnomemae
					   END as estnomemae,
					   CASE when estnomepai is null or estnomepai = '' then
								'IGNORADO'
							else
								estnomepai
					   END as estnomepai,
					   CASE when estsexo is null or estsexo = '' then
								'M'
							else
								estsexo
					   END as estsexo,
					   CASE when estnaturalidade is null or estnaturalidade = '' or estnaturalidade = 'B' then
								'1'
							else
								'2'
					   END as estnaturalidade,
					   CASE when estendcep is null or estendcep = '' then
								'70089000'
							else
								estendcep
					   END as estendcep,
					   '1' as esttipoendereco,
					   'R  ' as esttipologradouro,
					   CASE when estendlogradouro is null or estendlogradouro = '' then
								'S N'
							else
								estendlogradouro
					   END as estendlogradouro,
					   'NUM  ' as estsigla,
					   CASE when estendnumero is null or estendnumero = '' then
								'S N'
							else
								estendnumero
					   END as estposicao,
					   CASE when estendcomplemento is null or estendcomplemento = '' then
								'S N'
							else
								estendcomplemento
					   END as estendcomplemento,
					   CASE when estendbairro is null or estendbairro = '' then
								'S N'
							else
								estendbairro
					   END as estendbairro,
					   CASE when estendmuncod is null or estendmuncod = '' then
								'5300108'
							else
								estendmuncod
					   END as estendmuncod  
				from 
					projovemcampo.estudante 
				where 
					(estnis = '' or estnis is null or estnis = '0')
				order by estid
					LIMIT ".$_POST['ini']." OFFSET ".$_POST['fim']."
					";
		
		$dados2 = $db->carregar($sql);
		
		foreach($dados2 as $dados){
			/*
			$sql = "select estnome,to_char(estdatanascimento, 'DDMMYYYY') as estdatanascimento,
						   estnomemae,estnomepai,estsexo,caenaturalidade,
						   estcpf,
						   estendcep,
						   '1' as caetipoendereco,
						   'R  ' as caetipologradouro,
						   estendlogradouro,
						   'NUM. ' as caesigla,
						   caenumero as caeposicao,
						   estendcomplemento,
						   estendbairro,
						   muncod  
					from projovemcampo.cadastroestudante 
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
							 1 => str_pad(subistituiCaracteres($dados['estnome']), 212, " ", STR_PAD_RIGHT),
							 2 => str_pad(subistituiCaracteres($dados['estdatanascimento']), 212, " ", STR_PAD_RIGHT),
							 3 => str_pad(subistituiCaracteres($dados['estnomemae']), 212, " ", STR_PAD_RIGHT),
							 4 => str_pad(subistituiCaracteres($dados['estnomepai']), 212, " ", STR_PAD_RIGHT),
							 5 => str_pad(subistituiCaracteres($dados['estsexo']), 212, " ", STR_PAD_RIGHT),
							 6 => str_pad("0010", 212, " ", STR_PAD_RIGHT),
							 //7 => str_pad($dados['caenaturalidade'], 180, " ", STR_PAD_RIGHT),
							 7 => str_pad("1", 212, " ", STR_PAD_RIGHT),
							 8 => str_pad(subistituiCaracteres($dados['estcpf']), 212, " ", STR_PAD_RIGHT)
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
					
					
					//imprime
					echo $campo1.$campo2.$campo3.$campo4.$campo5.$campo6.$campo7.$campo8.$campo9.$campo10.$campo11.$campo12.$campo13;
					
					$seq++;
	
					//quebra linha
					if($i != 9) echo chr(13).chr(10);
					
					$linha++;
				}
			//Fim Dados do aluno
		
						 	
				
			//Endereço do aluno
			$valend = array(0 => str_pad(subistituiCaracteres($dados['estendcep']), 212, " ", STR_PAD_RIGHT),
							1 => str_pad(subistituiCaracteres($dados['esttipoendereco']), 212, " ", STR_PAD_RIGHT),
							2 => str_pad(subistituiCaracteres($dados['esttipologradouro']), 212, " ", STR_PAD_RIGHT),
							3 => str_pad(subistituiCaracteres($dados['estendlogradouro']), 212, " ", STR_PAD_RIGHT),
							4 => str_pad(subistituiCaracteres($dados['estsigla']), 212, " ", STR_PAD_RIGHT),
							5 => str_pad(subistituiCaracteres($dados['estposicao']), 212, " ", STR_PAD_RIGHT),
							6 => str_pad(subistituiCaracteres($dados['estendcomplemento']), 212, " ", STR_PAD_RIGHT),
							7 => str_pad(subistituiCaracteres($dados['estendbairro']), 212, " ", STR_PAD_RIGHT),
							8 => str_pad(subistituiCaracteres($dados['estendmuncod']), 212, " ", STR_PAD_RIGHT)
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

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

$menu = array(0 => array("id" => 1, "descricao" => "Gerar Arquivo CEF", 			"link" => "/projovemcampo/projovemcampo.php?modulo=principal/gerenciarNIS&acao=A"),
				  1 => array("id" => 2, "descricao" => "Carregar Arquivo NIS", 	"link" => "/projovemcampo/projovemcampo.php?modulo=principal/carregarNIS&acao=A")
			  	  );

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

monta_titulo('Projovem campo', 'Gerenciar NIS. Selecione os filtros abaixo.');

$bloq = 'S';
$disabled = '';
// $teste1 = 'áéãô.,@!$¨¨¨&()_';
// $teste = subistituiCaracteres($teste1);
// ver($teste1,$teste,d);
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/projovemcampo.js"></script>
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
			jQuery('#estnomesocial').val('');
		}
	});
});

function carregarMunicipios2(estuf) {
	var apcesfera = jQuery('#apcesfera').val();
	estuf = jQuery('#estuf').val();
	if( apcesfera == 'M' && estuf != '' ){
		jQuery('#tr_estendmuncod').show();
		jQuery('#td_estendmuncod').html('Carregando...');
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location,
	   		data: "req=carregarMunicipios2&estuf="+estuf,
	   		async: false,
	   		success: function(msg){jQuery('#td_estendmuncod').html(msg);}
	 	});
	}else if( apcesfera == 'E'){
		jQuery('#tr_estendmuncod').hide();
		jQuery('#td_estendmuncod').html('');
		if( estuf != '' ){
			carregarPolo(estuf);
		}
	}else if( estuf != '' ){
		alert('Escolha uma esfera.');
	}
}


function buscarentleos(polid, bloq) {
	if( polid != '' ){
		jQuery('td_entleo').html('Carregando...');
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location,
	   		data: "req=buscarentleos&polid="+polid+"&bloq=<?=$bloq ?>",
	   		async: false,
	   		success: function(msg){document.getElementById('td_entleo').innerHTML = msg;}
	 	});
	}
}

function buscarTurmas(entid, bloq) {
	if( entid != '' ){
		document.getElementById('td_turma').innerHTML = 'Carregando...';
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location,
	   		data: "req=buscarTurmas&entid="+entid+"&bloq=<?=$bloq ?>",
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
	location.href="projovemcampo.php?modulo=principal/gerenciarNIS&acao=A&qtd="+qtd;
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
				$sql = "SELECT 
							count(estid) as total  
						FROM 
							projovemcampo.estudante 
						WHERE 
							(estnis = '' or estnis is null)";
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
<?

// $where = Array();

// if( $_POST['apcesfera'] == 'E' ){
// 		$where[] = "apc.apcesfera = 'E'";
// }elseif( $_POST['apcesfera'] == 'M' ){
// 	$where[] = "apc.apcesfera = 'M'";
// }
// if( $_POST['status'] != '' && $_POST['status'] != ' ' ){
// 	$where[] = "est.eststatus = '".$_POST['status']."'";
// }
// if( $_POST['justificativaD'] ){
// 	$where[] = "est.estmotivoinativacao = 'DESISTENTE'";
// }
// if( $_POST['justificativaF'] ){
// 	$where[] = "est.estmotivoinativacao = 'FREQUENCIA INSUFICIENTE'";
// }
// if( $_POST['justificativaO'] ){
// 	$where[] = "est.estmotivoinativacao NOT IN ('DESISTENTE','FREQUENCIA INSUFICIENTE')";
// }
// if( $_POST['entid'] || $_POST['entid_disable'] ){
// 	$_POST['entid'] = $_POST['entid'] ? $_POST['entid'] : $_POST['entid_disable'];
// 	$where[] = "tur.entid = ".$_POST['entid'];
// }
// if( $_POST['turid'] ){
// 	$where[] = "tur.turid = ".$_POST['turid'];
// }
// if( $_POST['matricula'] ){
// 	$where[] = "2014||lpad(est.estid::varchar,6,'0') ilike '%".$_POST['matricula']."%'";
// }
// if( $_POST['estnome'] ){
// 	$where[] = "UPPER(est.estnome) ilike '%'||UPPER('".$_POST['estnome']."')||'%'";
// }
// if( $_POST['estcpf'] ){
// 	$where[] = "est.estcpf = '".str_replace(Array('.','-'),'',$_POST['estcpf'])."'";
// }

// $ano = 2014;

$acoes = "<input type=\"checkbox\" name=\"chkaluno[]\" id=\"chkaluno\" value=\"'||est.estid||'\">";
$acoes = " - ";

if(!$db->testa_superuser()) {
	
        $perfis = pegaPerfilGeral();
	
        
	if(in_array(PFL_SECRETARIO_MUNICIPAL, $perfis)) {
		$inner_municipio = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND c.apccodibge::character(7) = ur.muncod AND rpustatus='A'
						   	INNER JOIN rojovemcampo.secretario seco ON seco.secocpf = ur.usucpf";
		$where[] = "apc.apcesfera = 'M'";
		$acoes = '';
	}

	if(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
		$inner_municipio = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND sec.secoordcpf = ur.usucpf c.apccodibge::character(7) = ur.muncod AND rpustatus='A'";
		$where[] = "apc.apcesfera = 'M'";
		$acoes = '';
	}
	
	if(in_array(PFL_SECRETARIO_ESTADUAL, $perfis)) {
		$inner_estado = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND c.apccodibge::character(2) = esta.muncod AND rpustatus='A'
						 INNER JOIN rojovemcampo.secretario seco ON seco.secocpf = ur.usucpf";
		$where[] = "apc.apcesfera = 'E'";
		$acoes = '';
	}
	
	if(in_array(PFL_COORDENADOR_ESTADUAL, $perfis)) {
		$inner_estado = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND sec.secoordcpf = ur.usucpf c.apccodibge::character(2) = esta.muncod AND rpustatus='A'";
		$where[] = "apc.apcesfera = 'E'";
		$acoes = '';
	}
	
	if(in_array(PFL_COORDENADOR_TURMA, $perfis)) {
		$inner_estado = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.turid=tur.turid";
		$acoes = '';
	}
	
	if(in_array(PFL_DIRETOR_ESCOLA, $perfis)) {
		$inner_estado = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.entid=tur.entid";
		$acoes = '';
	}
	
	if(in_array(PFL_CONSULTA, $perfis)) {
		$acoes = '';
	}
}


//$acoes2 = "";

$sql = "SELECT DISTINCT
			'<center>$acoes</center>' as acoes,
			estnome,
			estcpf,
			--caeano||lpad(est.estid::varchar,6,'0') as ninscricao,	
			CASE WHEN apc.apcesfera = 'E' THEN 
					'ESTADUAL: '||upper(esta.estdescricao) 
				 ELSE 'MUNICIPAL: '||upper(mun.mundescricao)||' - '||mun.estuf 
			END as coord,
			entnome as entnome,
			turdescricao	
		FROM 
			projovemcampo.estudante est
		INNER JOIN projovemcampo.turma tur ON est.turid = tur.turid
		INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
		INNER JOIN projovemcampo.secretaria sec ON sec.secaid = tur.secaid
		INNER JOIN territorios.municipio mun ON mun.muncod = sec.secamuncod
		INNER JOIN territorios.estado esta ON esta.estuf = mun.estuf
		INNER JOIN entidade.entidade ent ON ent.entid = tur.entid
		$inner_estado
		$inner_municipio
		WHERE
			(est.estnis = '' or est.estnis is null)
		AND 1=1 ".(($where)?" AND ".implode(" AND ",$where) :"");
// ver($sql);
$cabecalho = array(" - ","Aluno","CPF","Matrícula","Coordenação","Pólo","Núcleo","Turma");
$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', $par2, 'formnis');
?>
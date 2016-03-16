<?php
 
set_time_limit(0);
ini_set("memory_limit", "2048M");
// ini_set("max_execution_time", "240");

// carrega as funções gerais
define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_SESSION['baselogin'] == "simec_desenvolvimento";

// require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'maismedicos/classes/Ws_Tutor.class.inc';

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$ws_tutor = new Ws_Tutor();

if($_GET['form']=='true'){
	
	if($_GET['conta']=='true'){ echo $db->pegaUm("select count(*) from maismedicos.ws_respostas_formulario where to_char(wssdata, 'YYYY-MM-DD') = '".date('Y-m-d')."'"); die('<p>die</p>'); }
	
	$ws_tutor->atualizaRespostasFormularioMaisMedicos();
	
}else if($_GET['item']=='true'){
	
	if($_GET['conta']=='true'){ echo $db->pegaUm("select count(*) from maismedicos.ws_respostas_formulario_itens where to_char(wssdata, 'YYYY-MM-DD') = '".date('Y-m-d')."'"); die('<p>die</p>'); }
	
	$ws_tutor->atualizaRespostasFormularioItensMaisMedicos();
}

die;

/*


getTime();
startExec();

if($_REQUEST['itens']){
	
	$ws_tutor->atualizaRespostasFormularioItensMaisMedicos();
	$tempo = endExec(false);
	
	$_SESSION['execucao_total'] += $tempo;
	echo 'Tempo de execução para carregar o(s) iten(s) de formulário: '.$tempo.'<br/>';
	echo 'Tempo de execução geral/parcial: '.$_SESSION['execucao_total'];
	echo '<p>&nbsp;</p>'; 
	die;
		
}else{
	
	echo '<p>######### INÍCIO ##########</p>';
	
	
	
	echo "<span id='qtde_forms'>";
	$ws_tutor->atualizaRespostasFormularioMaisMedicos();	
	$_SESSION['execucao_total'] = endExec(false);
	echo "</span> formulário(s) carregado(s) em {$_SESSION['execucao_total']} ms<br/>";	
	echo "<span id='qtde_form_processados'>0</span><p>&nbsp;</p><br/>";
	
	echo '<div id="console_txt"></div>';
	
	echo '<p>######### FIM ##########</p>';
}





// Funcoes de calculo de tempo de execução da página
global $time;

function getTime(){
	$microtime = explode(" ", microtime());
	$time = $microtime[0] + $microtime[1];
	return $time;
}

function startExec(){
	global $time;
	$time = getTime();
}

function endExec($txt = true){
	global $time;
	$finalTime = getTime();
	$execTime = $finalTime - $time;
	if($txt){
		echo 'Execution time: ' . number_format($execTime, 6) . ' ms';
	}else{
		return number_format($execTime, 6);
	}
}
*/

?>
<!--  
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

	var count = 0;
	var qtde_forms = $('#qtde_forms').text();
	
	function carregaItensFormulario()	
	{

		$.ajax({
			url		: document.location,
			type	: 'post',
			data	: 'itens=true',
			success	: function(e){
				
				++count;
				
				$('#console_txt').append(e);
				$('#qtde_form_processados').html(count+' formulários processados');
				
				if(parseInt(qtde_forms)<=parseInt(count)){
					clearInterval(itens);
					alert($('#console_txt').html());
				}
			}
		});

	}

	//var itens = window.setInterval( carregaItensFormulario, 3000 );
	carregaItensFormulario();

</script>
-->
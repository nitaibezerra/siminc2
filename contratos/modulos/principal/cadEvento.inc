<?php

if ($_REQUEST['pegadias']) {
    header('content-type: text/html; charset=ISO-8859-1');
    $datainicio = formata_data_sql($_POST['datainicio']);
    $datafim = formata_data_sql($_POST['datafim']);
    $dias = diffDate($datainicio, $datafim, 'D'); // coloquei uma função melhor!
    //$dias = dateDiff1($datainicio,$datafim); //comentei a função tosca do romualdo!
    echo $dias;
    exit;
}

function getDataFinalConvenio($dias, $data) {
    $arData = explode('/', $data);

    $dia = $arData[0];
    $mes = $arData[1];
    $ano = $arData[2];
    $dataFinal = mktime(24 * $dias, 0, 0, $mes, $dia, $ano);
    $dataFormatada = date('d/m/Y', $dataFinal);
    echo $dataFormatada;
}

function dateDiff1($dtinicio, $dtfim) {
    //$bissexto = date("L");
    //ver($dtinicio, $dtfim,d);
    /* if($bissexto == 1){
      $arDtInicio = explode( '-', $dtinicio );
      $arDtFim = explode( '-', $dtfim );

      ver($arDtInicio, $arDtFim,d);

      ver($dtinicio, $dtfim);
      } */

    $diff = strtotime($dtfim) - strtotime($dtinicio);
    $info = array();
    if ($diff > 86400) {
        $info['d'] = ($diff - ($diff % 86400)) / 86400;
        $diff = $diff % 86400;
    }
    $f = '';
    foreach ($info as $k => $v) {
        if ($v > 0)
            $f .= "$v";
    }
    return ($f == 24) ? "1" : $f;
}

function diffDate($d1, $d2, $type = '', $sep = '-') {
    $d1 = explode($sep, $d1);
    $d2 = explode($sep, $d2);
    switch ($type) {
        case 'A':
            $X = 31536000;
            break;
        case 'M':
            $X = 2592000;
            break;
        case 'D':
            $X = 86400;
            break;
        case 'H':
            $X = 3600;
            break;
        case 'MI':
            $X = 60;
            break;
        default:
            $X = 1;
    }
    $t1 = mktime(0, 0, 0, $d2[1], $d2[2], $d2[0]);
    $t2 = mktime(0, 0, 0, $d1[1], $d1[2], $d1[0]);
    $r = $t1 - $t2;
    return floor($r / $X) + 1; //Somo 1 pois conta tanto a data inicial quanto a final, não é o tempo de uma pra outra e sim a quantidade de dias contanto a primeira e a ultima datas!
}

if ($_REQUEST['mostraUnParceiras']) {

    $ungcod = $_REQUEST['ungcod'];

    $sql = sprintf("SELECT  ungcod AS codigo, 
                            ungabrev || ' - ' || ungdsc AS descricao
                    FROM public.unidadegestora
                    WHERE ungcod <> '%s'
                    ORDER BY ungdsc", $ungcod);

    combo_popup('unpcod', $sql, 'Selecione a(s) Unidade(s) Parceira(s)', '360x460', '', '', '', 'S', '', '', 6, 400, '', '');
    die();
}

if($_REQUEST['mostraPregoes']) {
	$ungcod = $_REQUEST['ungcod'];
	
	/*
	 * Comentado a pedido do Juvenal, o sistema nunca apresentará mais de um pregão ativo.
	$sql = sprintf("SELECT 
                            ur.ureid AS codigo, 
                            precodpregao || ' - ' || predescpregao || ' (' || prerazaosocial || ')' AS descricao
                        FROM
                            evento.unidaderecurso ur 
                        INNER JOIN 
                        	evento.pregaoevento pe ON ur.preid = pe.preid 
                        WHERE ur.ungcod = '%s'
                        ORDER BY descricao", $ungcod);
	 
	$db->monta_combo('ureid', $sql, 'S', "Selecione...", '', '', '', '300', 'S', 'ureid', '', $_REQUEST['ureidsel']);
	*/
	
	$sql = sprintf("SELECT 
                            ur.ureid AS codigo  
                        FROM
                            evento.unidaderecurso ur 
                        INNER JOIN 
                        	evento.pregaoevento pe ON ur.preid = pe.preid 
                        WHERE ur.ungcod = '%s' AND pe.prestatus='A'", $ungcod);
	$ureid = $db->pegaUm($sql);
	echo "<input type=\"hidden\" name=\"ureid\" id=\"ureid\" value=\"".$ureid."\">";
	die();
}

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$perfis = arrayPerfil();

if(in_array(PERFIL_EMPRESA, $perfis) &&
 	count($perfis) == 1){
 		echo "<script>document.location.href = 'evento.php?modulo=principal/cadEventoAnexo&acao=A'</script>";
}

if( $_SESSION['eveid'] != ''){
	$docid = evtCriarDoc( $_SESSION['eveid'] );
}
 
if( $_POST['ajaxunsetsession'] == '1'){
	$_SESSION['eveid'] = ''; 
	die();
}
 
include_once APPRAIZ . "includes/classes/dateTime.inc";
 
if( $_POST['action'] == 'grava'){

	if( $_SESSION['eveid']){
		salvar( $_SESSION['eveid'] );
	} else {
		salvar();
	}
}

if( $_POST['ajaxestuf'] ){	 
	header('content-type: text/html; charset=ISO-8859-1');
 
	$sql = "select
			 muncod as codigo, mundescricao as descricao 
			from
			 territorios.municipio 
			where
			 estuf = '".$_POST['ajaxestuf']."' 
			order by
			 mundescricao asc";
	die($db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', '', '', 'muncod' ));
}

if( $_SESSION['eveid'] != '')
{ 
	$sql = "
            SELECT  ev.evetitulo,  
                    ev.ungcod,
                    ev.tpeid,			 
                    ev.evedatainicio,		 
                    ev.evedatafim,			 
                    ev.eveemail,
                    ev.everespnome,
                    ev.everesptelefone, 		 
                    ev.evecustoprevisto, 	 
                    ev.evepublicoestimado,  
                    ev.evequantidadedias,
                    ev.muncod, 				 
                    ev.estuf, 				 
                    ev.sevid,
                    ev.eveqtdpassagemaerea,
                    u.ungdsc,
                    us.usunome,
                    ev.docid,
                    ev.eveurgente,
                    aval.aevid,
                    ev.endid,
                    to_char(ev.evedatainclusao::date,'DD/MM/YYYY') AS evedatainclusao,
                    ev.ureid,
                    ev.evelocal,
                    ev.evenumeroprocesso,
                    ev.evedemandante,
                    ev.evecargodemandante
            FROM evento.evento AS ev
            LEFT JOIN evento.tipoevento AS    te ON te.tpeid  = ev.tpeid
            LEFT JOIN public.unidadegestora AS u ON ev.ungcod = u.ungcod
            LEFT JOIN seguranca.usuario AS    us ON us.usucpf = ev.usucpf
            LEFT JOIN evento.avaliacaoevento aval ON aval.eveid = ev.eveid 
            WHERE ev.eveid = {$_SESSION['eveid']}
        ";
        $rsDadosEvento = $db->carregar($sql);
}

function salvar($id = null) {
    global $db;

    $endereco = new Endereco($_REQUEST['endereco']['endid']);
    $entidade->enderecos[0] = $endereco;

    $locnome = $_REQUEST['locnome'];
    $endcep = str_replace(array(".", "-"), '', $_REQUEST['endereco']['endcep']);
    $endlog = $_REQUEST['endereco']['endlog'];
    $endcom = $_REQUEST['endereco']['endcom'];
    $endbai = $_REQUEST['endereco']['endbai'];
    $muncod = $_REQUEST['endereco']['muncod'];
    $estuf = $_REQUEST['endereco']['estuf'];
    $endnum = $_REQUEST['endereco']['endnum'];
    $endstatus = $_REQUEST['endereco']['endstatus'];

    $medlatitude = $_REQUEST['graulatitude'] . '.' . $_REQUEST['minlatitude'] . '.' . $_REQUEST['seglatitude'] . '.' . $_REQUEST['pololatitude'];
    $medlongitude = $_REQUEST['graulongitude'] . '.' . $_REQUEST['minlongitude'] . '.' . $_REQUEST['seglongitude'];

    $data = new Data();
    $retorno = $data->timeStampDeUmaData(date("d/m/Y"));
    $retorno1 = $data->timeStampDeUmaData($_REQUEST['evedatainicio']);
    $segundos_diferenca = $retorno - $retorno1;
    
    $dias_diferenca = $segundos_diferenca / (60 * 60 * 24);
    $dias_diferenca = abs($dias_diferenca);
    $dias_diferenca = floor($dias_diferenca);
    
    $evedemandante      = $_POST['evedemandante'];
    $evecargodemandante = $_POST['evecargodemandante'];

    if ($_REQUEST['evepublicoestimado'] <= 50) {
        $diferenca_permitida = 30;
    } elseif (( $_REQUEST['evepublicoestimado'] > 50 ) AND ( $_REQUEST['evepublicoestimado'] <= 250 )) {
        $diferenca_permitida = 45;
    } elseif (( $_REQUEST['evepublicoestimado'] > 250 ) AND ( $_REQUEST['evepublicoestimado'] <= 500 )) {
        $diferenca_permitida = 60;
    } elseif ($_REQUEST['evepublicoestimado'] > 500) {
        $diferenca_permitida = 90;
    }

    if ($dias_diferenca >= $diferenca_permitida) {
        $evedataurgente = "f";
        //$eveurgente     = "f";
    } else {
        $evedataurgente = "t";
        //$eveurgente     = "t";
    }

    #verificando se valerá a regra de AD-REFERENDUM para o perfil.
//	$perfis = arrayPerfil();
//	if( !in_array( PERFIL_SUPER_USUARIO, $perfis) && !in_array(PERFIL_SAA, $perfis)){
//		if( $adreferendum != $evedataurgente){
//			return false;
//		}	
//	} 
    
    $arrDatamI = explode("/", $_REQUEST['evedatainicio']);
    $formataDatamI = $arrDatamI[2] . '-' . $arrDatamI[1] . '-' . $arrDatamI[0];

    $arrDatamF = explode("/", $_REQUEST['evedatafim']);
    $formataDatamF = $arrDatamF[2] . '-' . $arrDatamF[1] . '-' . $arrDatamF[0];

    if ($_POST['eveqtdpassagemaerea'] == '') {
        $eveqtdpassagemaerea = 0;
    } else {
        $eveqtdpassagemaerea = $_POST['eveqtdpassagemaerea'];
    }

    if ($id == NULL) {
        $sqlEnd = "
            INSERT INTO 
                entidade.endereco (
                        endcep, 
                        endlog, 
                        endcom, 
                        endbai, 
                        muncod,  
                        estuf, 
                        endnum, 
                        endstatus,  
                        medlatitude, 
                        medlongitude
                )VALUES (                                                   
                        '$endcep', 
                        '$endlog', 
                        '$endcom', 
                        '$endbai', 
                        '$muncod',  
                        '$estuf', 
                        '$endnum', 
                        '$endstatus',  
                        '$medlatitude',
                        '$medlongitude'
                ) RETURNING endid
        ";
        $endid = $db->pegaUm($sqlEnd);
        $db->commit();

        $sql = "
            INSERT INTO 
                evento.evento(
                    evetitulo ,
                    ungcod,
                    tpeid,
                    evedatainicio,
                    evedatafim,
                    eveemail,
                    everespnome,
                    everesptelefone,
                    evequantidadedias,
                    evepublicoestimado,
                    eveqtdpassagemaerea,
                    evedatainclusao,
                    muncod,
                    estuf,
                    --eveurgente,
                    sevid ,
                    evestatus,
                    usucpf,
                    endid,
                    ureid,
                    evenumeroprocesso,
                    evedemandante,
                    evecargodemandante
                )VALUES(
                    '".$_POST['evetitulo']."',
                    '".$_POST['ungcod']."',
                    '".$_POST['tpeid']."',
                    '".$formataDatamI."',
                    '".$formataDatamF."',
                    '".$_POST['eveemail']."',
                    '".$_POST['everespnome']."',
                    '".$_POST['everesptelefone']."',
                    '".$_POST['evequantidadedias']."',
                    '".$_POST['evepublicoestimado']."',
                    '".$eveqtdpassagemaerea."',
                    'now();',
                    '".$_POST['muncod']."',
                    '".$_POST['estuf_disable']."',
                    2,
                    'A',
                    '".$_SESSION['usucpf']."',
                    '".$endid."',
                    '".$_REQUEST['ureid']."',
                    ".( strlen($_POST['evenumeroprocesso']) != 20 ? "null" : "'" . $_POST['evenumeroprocesso'] . "'" ).",
                    '".$evedemandante."',
                    '".$evecargodemandante."'
            )RETURNING eveid
        ";

        if ($eveid = $db->pegaUm($sql)) {
            if (!empty($_REQUEST['unpcod'][0])) {
                foreach ($_REQUEST['unpcod'] as $ungcod) {
                    $sql = "INSERT INTO evento.unidadeparceira (eveid, ungcod) VALUES ('{$eveid}', '{$ungcod}')";
                    $db->executar($sql);
                }
            }
            $db->commit();
            $_SESSION['eveid'] = $eveid;
            $docid = evtCriarDoc($eveid);
            echo"<script>alert('Evento gravado com sucesso!');window.location.href = 'evento.php?modulo=principal/cadEvento&acao=A';</script>";
        }
    }else{
        if (!$_POST['endereco']['endid']) {
            $sqlEnd = "
                    INSERT INTO                                                   
                        entidade.endereco(
                            endcep, 
                            endlog, 
                            endcom, 
                            endbai, 
                            muncod,  
                            estuf, 
                            endnum, 
                            endstatus,  
                            medlatitude, 
                            medlongitude                              
                        )VALUES(                                                   
                            '$endcep', 
                            '$endlog', 
                            '$endcom', 
                            '$endbai', 
                            '$muncod',  
                            '$estuf', 
                            '$endnum', 
                            '$endstatus',  
                            '$medlatitude',
                            '$medlongitude'
                         ) RETURNING endid
            ";
            $endid = $db->pegaUm($sqlEnd);
        }else{

            $endid = $_REQUEST['endereco']['endid'];

            $sqlEnd = "
                UPDATE entidade.endereco 
                    SET endcep = '$endcep', 
                        endlog = '$endlog', 
                        endcom = '$endcom', 
                        endbai = '$endbai', 
                        muncod = '$muncod',  
                        estuf  = '$estuf', 
                        endnum = '$endnum', 
                        endstatus    = '$endstatus',  
                        medlatitude  = '$medlatitude', 
                        medlongitude = '$medlongitude'
                WHERE endid = '{$_REQUEST['endereco']['endid']}'
            ";
            $db->executar($sqlEnd);
        }

        if (!empty($_REQUEST['unpcod'][0])) {
            foreach ($_REQUEST['unpcod'] as $ungcod) {
                $sql = "INSERT INTO evento.unidadeparceira (eveid, ungcod) VALUES ({$_SESSION['eveid']}, '{$ungcod}')";
                $db->executar($sql);
            }
        }

        $sql = "
            UPDATE evento.evento
                SET evetitulo           = '".$_POST['evetitulo']."',
                    ungcod 		= '".$_POST['ungcod']."',
                    tpeid 		= '".$_POST['tpeid']."',
                    evedatainicio	= '".$formataDatamI."',
                    evedatafim		= '".$formataDatamF."',
                    eveemail 		= '".$_POST['eveemail'] . "',
                    everespnome 	= '".$_POST['everespnome'] . "',
                    everesptelefone 	= '".$_POST['everesptelefone'] . "',
                    evequantidadedias   = '".$_POST['evequantidadedias']."',
                    evepublicoestimado  = '".$_POST['evepublicoestimado'] . "',
                    eveqtdpassagemaerea = '".$eveqtdpassagemaerea."',
                    muncod 		= '".$_POST['muncod']."',
                    estuf 		= '".$_POST['estuf_disable']."',
                    --eveurgente	= '".$eveurgente."',
                    sevid		= 2,
                    evestatus		= 'A',
                    endid		= '". $endid."',
                    ureid		= '". $_POST['ureid']."',
                    --evelocal		= '". $_POST['evelocal']."',
                    evenumeroprocesso   = ".( strlen($_POST['evenumeroprocesso']) != 20 ? "null" : "'" . $_POST['evenumeroprocesso']."'").",
                    evedemandante       = '".$evedemandante."',
                    evecargodemandante  = '".$evecargodemandante."'    
            WHERE eveid = '" . $_SESSION['eveid'] . "'
	";

        if ($db->executar($sql)) {
            $db->commit();
            echo"<script>alert('Evento alterado com sucesso!');window.location.href = 'evento.php?modulo=principal/cadEvento&acao=A';</script>";
        }
    }
    return true;
}

$titulo_modulo = "Eventos";
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( $titulo_modulo, 'Solicitar Pré-agendamento de eventos.' );

?>


<script src="../includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>

jQuery.noConflict();

function diasDecorridos(dt1, dt2){
    // variáveis auxiliares
    var minuto = 60000; 
    var dia = minuto * 60 * 24;
    var horarioVerao = 0;
    
    // ajusta o horario de cada objeto Date
    dt1.setHours(0);
    dt1.setMinutes(0);
    dt1.setSeconds(0);
    dt2.setHours(0);
    dt2.setMinutes(0);
    dt2.setSeconds(0);
    
    // determina o fuso horário de cada objeto Date
    var fh1 = dt1.getTimezoneOffset();
    var fh2 = dt2.getTimezoneOffset(); 
    
    // retira a diferença do horário de verão
    if(dt2 > dt1){
      horarioVerao = (fh2 - fh1) * minuto;
    } 
    else{
      horarioVerao = (fh1 - fh2) * minuto;    
    }
    
    var dif = Math.abs(dt2.getTime() - dt1.getTime()) - horarioVerao;
    return Math.ceil(dif / dia);
}

function salvaEvento(tipo){

        var datai               = document.getElementById('evedatainicio');
        var dataf 		= document.getElementById('evedatafim');	
        var evetitulo 		= document.getElementById('evetitulo');
        var ungcod 		= document.getElementById('ungcod');
        var tpeid 		= document.getElementById('tpeid');
        var evequantidadedias   = document.getElementById('evequantidadedias');
        var hidquantidadedias   = document.getElementById('hid_evequantidadedias');
        var eveemail 		= document.getElementById('eveemail');
        var everesptelefone	= document.getElementById('everesptelefone');
        var everespnome		= document.getElementById('everespnome');
        var evepublicoestimado  = document.getElementById('evepublicoestimado');
        var muncod 		= document.getElementById('muncod');
        var estuf 		= document.getElementById('estuf');
        //var evelocal		= document.getElementById('evelocal');
        var evenumeroprocesso   = document.getElementById('evenumeroprocesso');
        
        var evedemandante       = document.getElementById('evedemandante');
        var evecargodemandante  = document.getElementById('evecargodemandante');

        if(document.getElementById('eveqtdpassagemaerea')){		
                var eveqtdpassagemaerea = document.getElementById('eveqtdpassagemaerea');
        }
        var passagens = document.getElementsByName('passagens');

        var tipo;

        if( tipo == 'grava' ){
            if( evetitulo.value == '')
            {
                    alert('O Campo "Título" é Obrigatório');
                    document.formulario.evetitulo.focus();
                    return false;
            }

            if( ungcod.value == '')
            {
                    alert('O Campo "Unidade Gestora" é Obrigatório');
                    document.formulario.ungcod.focus();
                    return false;
            }

            if( evedemandante.value == ''){
                    alert('O Campo "Nome do Demandante" é Obrigatório');
                    document.formulario.ungcod.focus();
                    return false;
            }
            
            if( evecargodemandante.value == ''){
                    alert('O Campo "Cargo do Demandante" é Obrigatório');
                    document.formulario.ungcod.focus();
                    return false;
            }            

            if( tpeid.value == '')
            {
                    alert('O Campo "Tipo de Evento" é Obrigatório');
                    document.formulario.tpeid.focus();
                    return false;
            }

            if( evequantidadedias.value == '')
            {
                    alert('O Campo "Nº de dias do Evento" é Obrigatório');
                    document.formulario.evequantidadedias.focus();
                    return false;
            }

            if( hidquantidadedias.value != 'f' )
            {
                    if( ( Number(hidquantidadedias.value ) ) > (Number( evequantidadedias.value )) )
                    {
                            if ( !confirm( 'A quantidade de dias informada para o evento é menor do que a cadastrada no sistema. A diminuição dos dias acarretará em perdas de informações declaradas na tela de infraestrutura. \nDeseja continuar esta operação? ' ) )
                            {
                                    document.formulario.hidquantidadedias.focus();
                                    return false;
                            } 
                    }
            }

            if( datai.value == '')
            {
                    alert('O Campo "Data de Início do Período do Evento" é Obrigatório');
                    document.formulario.evedatainicio.focus();
                    return false;
            }
            if( dataf.value == '')
            {
                    alert('O Campo "Data de termino do Período do Evento" é Obrigatório');
                    document.formulario.evedatafim.focus();
                    return false;
            }

            var datainicio = document.getElementById('evedatainicio').value;
            var datafim = document.getElementById('evedatafim').value;
            var dias = '';
            if( datainicio != '' && datafim != '' ){
                    var myajax = new Ajax.Request('evento.php?modulo=principal/cadEvento&acao=A', {
                            method:     'post',
                            parameters: '&pegadias=true&datafim='+datafim+'&datainicio='+datainicio,
                            asynchronous: false,
                            onComplete: function (res){
                                    dias = res.responseText;
                            }
                      });
            }

            if( dias != (parseInt(evequantidadedias.value)) )
            {
                    alert('O Campo "Nº de dias do Evento" está incompatível com o "Período do Evento".');
                    document.formulario.evedatafim.focus();
                    return false;
            }

            if(!validaDataMaior(datai, dataf)){
                alert('A data de início não pode ser maior do que a data final do evento.');
                dataf.focus();
                dataf.value = ''; 
                return false;
            }
    /*	
    if( evelocal.value == '')
            {
                    alert('O Campo "Local do Evento:" é Obrigatório');
                    document.formulario.evelocal.focus();
                    return false;
            }
            */	
            if( evepublicoestimado.value == '')
            {
                    alert('O Campo "Publico:" é Obrigatório');
                    document.formulario.evepublicoestimado.focus();
                    return false;
            }

            var arDatai = datai.value.split('/');
            var dataAtual = new Date();

            var data1 = new Date(parseInt(arDatai[2]), parseInt(arDatai[1])-1, parseInt(arDatai[0]));		
            var data2 = new Date(dataAtual.getFullYear(), dataAtual.getMonth(), dataAtual.getDate());

            var dif = Date.UTC(data1.getYear(),data1.getMonth(),data1.getDate(),0,0,0) - Date.UTC(data2.getYear(),data2.getMonth(),data2.getDate(),0,0,0);
            var dias_ate_evento = Math.abs((dif / 1000 / 60 / 60 / 24));
            /*
            if(evepublicoestimado >= 0 && evepublicoestimado <= 50){
                    if(dias_ate_evento < 30){
                            alert('Evento com até 50 participantes o prazo de envio para análise do comitê é 30 (trinta) dias!');
                            return false;
                    }
            }
            if(evepublicoestimado >= 51 && evepublicoestimado <= 250){
                    if(dias_ate_evento < 45){
                            alert('Evento com até 250 participantes o prazo de envio para análise do comitê é 45 (quarenta e cinco) dias!');
                            return false;
                    }
            }
            if(evepublicoestimado >= 251 && evepublicoestimado <= 500){
                    if(dias_ate_evento < 60){
                            alert('Evento com até 500 participantes o prazo de envio para análise do comitê é 60 (sessenta) dias!');
                            return false;
                    }
            }
            if(evepublicoestimado > 500){
                    if(dias_ate_evento < 90){
                            alert('Evento com mais de 500 participantes o prazo de envio para análise do comitê é 90 (noventa) dias!');
                            return false;
                    }
            }
            */
            if( estuf.value == '')
            {
                    alert('O Campo "UF" é Obrigatório');
                    document.formulario.estuf.focus();
                    return false;
            }
            if( muncod.value == '')
            {
                    alert('O Campo "Município" é Obrigatório');
                    document.formulario.muncod.focus();
                    return false;
            }

            if( evenumeroprocesso.value == '')
            {
                    alert('O Campo "Nº Processo" é Obrigatório');
                    document.formulario.evenumeroprocesso.focus();
                    return false;
            }

            if(evenumeroprocesso.value.length != 20 && 
               evenumeroprocesso.value.search("/") != 12 && 
               evenumeroprocesso.value.search("/") != 17){
                    alert('Número de Processo inválido!');
                    document.formulario.evenumeroprocesso.focus();
                    return false;
            }

            if( everespnome.value == '')
            {
                    alert('O Campo "Nome do Responsável" é Obrigatório');
                    document.formulario.everespnome.focus();
                    return false;
            }
            if( everesptelefone.value == '')
            {
                    alert('O Campo "Telefone" é Obrigatório');
                    document.formulario.everesptelefone.focus();
                    return false;
            }
            if( eveemail.value == '')
            {
                    alert('O Campo "Email" é Obrigatório');
                    document.formulario.eveemail.focus();
                    return false;
            }

            if( passagens[0].checked )
            {
                    if( eveqtdpassagemaerea.value == '' )
                    {
                            alert('A quantidade de "Passagens Aéreas" é Obrigatória');
                            eveqtdpassagemaerea.focus();
                            return false;
                    }
            }

            document.getElementById('action').value=tipo;		
            selectAllOptions(document.getElementById('unpcod'));		
            document.formulario.submit();		
        }

        if( tipo == 'cancel' )
        {
                window.location.href= "evento.php?modulo=inicio&acao=C";
        }

}

function alertaPI(){
	alert("Escolha um ano para listar os PIs.");
	document.getElementById('anoPI').focus();
	return false;
}

function pegaUnidadesParceiras(){
	
	var tdUP = document.getElementById('unParceiras');
	
	new Ajax.Request('evento.php?modulo=principal/cadEvento&acao=A', {
					
					method: 'post',
	 				parameters: '&mostraUnParceiras=true&ungcod='+document.formulario.ungcod.value,							         
			        onComplete: function (res)
			        { 
						tdUP.innerHTML = res.responseText; 
			        }			
	});
	
	pegaPregoes();	
}

function pegaPregoes() {

	var tdPR = document.getElementById('pregoes');

	new Ajax.Request('evento.php?modulo=principal/cadEvento&acao=A', {
					
					method: 'post',
					asynchronous: false,
	 				parameters: '<?=(($ureid)?"&ureidsel=".$ureid:"") ?>&mostraPregoes=true&ungcod='+document.formulario.ungcod.value,							         
			        onComplete: function (res)
			        { 
						tdPR.innerHTML = res.responseText; 
			        }			
	});

}

function dateDiff(datepart, fromdate, todate)// datepart: 'w', 'd', 'h', 'n', 's'
{
	datepart = datepart.toLowerCase();
	var diff = todate - fromdate;
	var divideBy = {
		  w:604800000 //1000*60*60*24*7
		, d:86400000 // 1000*60*60*24
		, h:3600000 // 1000*60*60
		, n:60000 // 1000*60
		, s:1000
	};
	return Math.floor(diff/divideBy[datepart]);
}

function filtraMunicipio(estuf)
{
	//alert( estuf);
	td 	   = document.getElementById('municipio');
	select = document.getElementsByName('muncod')[0];
	
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('evento.php?modulo=principal/cadEvento&acao=A', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,							         
							        onComplete: function (res)
							        {			
										td.innerHTML = res.responseText;
										td.style.visibility = 'visible';
							        }
							        
							  });
    
}
 
function setPassagem( value )
{
	var lb_sim = document.getElementById('lb_sim');
	if( value == 't'){
		var content = " - Quantidade: <input type='text' value='' size='4' onkeyup=\"this.value=mascaraglobal('#####',this.value);\" name='eveqtdpassagemaerea' id='eveqtdpassagemaerea'><img src='../imagens/obrig.gif' title='Indica campo obrigatório.' border='0'>";
		lb_sim.innerHTML = content;
		top.temPassagem = 's';
	}
	else{
		 lb_sim.innerHTML = '';
		 top.temPassagem = 'n';
	}
}	 

function abreMapa(){
	var graulatitude = window.document.getElementById("graulatitude").value;
	var minlatitude  = window.document.getElementById("minlatitude").value;
	var seglatitude  = window.document.getElementById("seglatitude").value;
	var pololatitude = window.document.getElementById("pololatitude").value;
	
	var graulongitude = window.document.getElementById("graulongitude").value;
	var minlongitude  = window.document.getElementById("minlongitude").value;
	var seglongitude  = window.document.getElementById("seglongitude").value;
	
	var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
	var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
	var eveid = document.getElementById("eveid").value;
	var janela=window.open('evento.php?modulo=principal/mapaEvento&acao=A&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude+'&eveid='+eveid, 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();

}

function validaMascaraProcesso(valor)
{
	if(valor.length != 20 && valor.search("/") != 12 && valor.search("/") != 17)
	{
		alert('Número de Processo inválido!');
	}
}

<? if($ungcod): ?>
	document.observe("dom:loaded", function() {
		pegaPregoes();
		if(document.getElementById('btnGravar').disabled) {
			document.getElementById('ureid').disabled=true;
		}
	});
<? endif; ?>

</script> 


<?php

$res = array(
    0 => array("descricao" => "Lista",
        "id" => "4",
        "link" => "/evento/evento.php?modulo=inicio&acao=C&submod=evento"
    ),
    1 => array("descricao" => "Informações Básicas",
        "id" => "4",
        "link" => "/evento/evento.php?modulo=principal/cadEvento&acao=A"
    )
);

if (( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '')) {
    array_push($res, array("descricao" => "Documentos Anexos",
        "id" => "3",
        "link" => "/evento/evento.php?modulo=principal/cadEventoAnexo&acao=A"
            ), array("descricao" => "Infraestrutura",
        "id" => "2",
        "link" => "/evento/evento.php?modulo=principal/cadEventoInfra&acao=A"
            )
    );

    $pflcod = pegaPerfil($_SESSION['usucpf']);
    /*
      if( ( $pflcod == PERFIL_SUPER_USUARIO) || ( $pflcod == PERFIL_SAA ) )
      {
      array_push($res,
      array ("descricao" => "Hotéis",
      "id"		=> "1",
      "link"		=> "/evento/evento.php?modulo=principal/cadEventoHotel&acao=A"
      )
      );
      }
     */
}
if ($_SESSION['eveid']) {
    

    /* RETIRANDO ABA "Estrutura Orçamentária" conforme solicitado na demanda 209333 item 8
     * if ($_SESSION['eveid']) {
        array_push($res, array("descricao" => "Estrutura Orçamentária",
            "id" => "6",
            "link" => "/evento/evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A"
                )
        );
    }
    */

    if (mostraAbaDocOS($_SESSION['eveid'])) {
        array_push($res, array("descricao" => "Ordem de Serviço",
            "id" => "7",
            "link" => "/evento/evento.php?modulo=principal/cadOrdemServico&acao=A"
                )
        );
    }
    
	if (mostraAbaDocPagamento($_SESSION['eveid'])) {
        array_push($res, array("descricao" => "Documento de Pagamento",
            "id" => "5",
            "link" => "/evento/evento.php?modulo=principal/cadDocPagamento&acao=A"
                )
        );
    }
}


if (( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '')) {
    array_push($res, array("descricao" => "Avaliação",
        "id" => "0",
        "link" => "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A"
            )
    );
}

if ($_SESSION['eveid']) {
    echo'<br><br>';
    echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=principal/cadEvento&acao=A");
}
?>
   
<form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class ="SubTituloDireita" align="right">Título do Evento: 
                <input type = "hidden" name="update" id="update" value="0" />
                <input type = "hidden" name="insert" id="insert" value="0" />
            </td>
            <td> 
                <? $evetitulo = $rsDadosEvento[0]['evetitulo']; ?>
                <?= campo_texto('evetitulo', 'S', $somenteLeitura, '', 40, 300, '', '', 'left', '', 0, 'id="evetitulo" onblur="MouseBlur(this);"'); ?>               
            </td>
            <td rowspan="14" style="background-color:#fafafa; color:#404040; width: 1px;" valign="top" align="left">
                <?php
                // Barra de estado atual e ações e Historico
                if ($_SESSION['eveid'] != '') {
                    wf_desenhaBarraNavegacao($docid, array('' => ''));
                }
                ?>		
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Unidade Gestora: </td>
            <td> 
                <?php
                $sql = "SELECT 
                            ug.ungcod AS codigo, 
                            ug.ungabrev || ' - ' || ug.ungdsc AS descricao
                        FROM
                            public.unidadegestora ug
                        INNER JOIN 
                            evento.unidaderecurso ur ON ur.ungcod = ug.ungcod  
                        INNER JOIN 
                        	evento.pregaoevento pe ON pe.preid = ur.preid 
                        WHERE 
                        	prestatus='A'
                        ORDER BY 
                        	ungdsc
                       ";

                $ungcod = $rsDadosEvento[0]["ungcod"];

                $db->monta_combo('ungcod', $sql, 'S', "Selecione...", 'pegaUnidadesParceiras', '', '', '200', 'S', 'ungcod');
                $ureid = $rsDadosEvento[0]["ureid"];
                ?>
                <div id="pregoes"><input type="hidden" name="ureid" id="ureid" value="<?= $ureid ?>"></div>     
            </td> 
        </tr>
        <!--Demandante-->
        <tr>
            <td class ="SubTituloDireita" align="right">Nome do Demandante:</td>
            <td> 
                <?php
                    $evedemandante = $rsDadosEvento[0]['evedemandante'];
                    echo campo_texto('evedemandante', 'S', $somenteLeitura, '', 40, 200, '', '', 'left', '', 0, 'id="evedemandante"', '', $evedemandante); 
                ?>               
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Cargo do Demandante:</td>
            <td> 
                <?php
                    $evecargodemandante = $rsDadosEvento[0]['evecargodemandante'];
                    echo campo_texto('evecargodemandante', 'S', $somenteLeitura, '', 40, 200, '', '', 'left', '', 0, 'id="evecargodemandante"', '', $evecargodemandante); 
                ?>               
            </td>
        </tr>
        <!--FIM Demandante-->
        <!-- 
        Esta sendo comentado por o Juvenal solicitou a mudança na forma de selecionar o pregão, nunca existirá mais de um pregão ativo
        <tr>
            <td class ="SubTituloDireita" align="right">Pregão: </td>
            <td> 
        <?php
        //$db->monta_combo('ureid', $sql=array(), 'S', "Selecione...", '', '', '', '200', 'S', 'ureid');
        ?>     
            </td> 
        </tr>
        -->          
        <tr>
            <td align='right' class="SubTituloDireita">Unidade(s) Parceira(s):</td>
            <td id="unParceiras">
                <?php
                    $sql = sprintf("SELECT 
                                        ungcod AS codigo, 
                                        ungabrev || ' - ' || ungdsc AS descricao
                                    FROM
                                        public.unidadegestora
                                    WHERE ungcod <> '%s'
                                    ORDER BY ungdsc", $ungcod);

                    if ($_SESSION['eveid']) {
                        $sql_seleciona = sprintf("SELECT 
                                                    ug.ungcod AS codigo, 
                                                    ungabrev || ' - ' || ungdsc AS descricao
                                                FROM public.unidadegestora ug
                                                INNER JOIN evento.unidadeparceira up ON up.ungcod = ug.ungcod
                                                WHERE up.eveid = '%s'					                         
                                                ORDER BY ungdsc", $_SESSION['eveid']);
                        $nome = 'unpcod';
                        $$nome = $db->carregar($sql_seleciona);
                    }
                    combo_popup('unpcod', $sql, 'Selecione a(s) Unidade(s) Parceira(s)', '360x460', '', '', '', 'S', '', '', 4, 400, '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Tipo: </td>
            <td> 
                <?php
                $sql = "SELECT 
                        tpeid AS codigo, 
                        tpedescricao AS descricao
                    FROM
                        evento.tipoevento
                   ";

                $tpeid = $rsDadosEvento[0]["tpeid"];

                $db->monta_combo('tpeid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'tpeid');
                ?> 
            </td> 
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Nº de dias do Evento: </td>
            <td> 
                <?
                    if (trim($rsDadosEvento[0]['evequantidadedias']) != '') {
                ?> 
                    <input type="hidden" name="hid_evequantidadedias" value="<?= $rsDadosEvento[0]['evequantidadedias']; ?>" id="hid_evequantidadedias">                    	
                <?
                } else {
                ?>
                    <input type="hidden" name="hid_evequantidadedias" value="f" id="hid_evequantidadedias">                    	
                <?
                }
                $evequantidadedias = $rsDadosEvento[0]['evequantidadedias'];
                echo campo_texto('evequantidadedias', 'S', 'S', '', 5, 3, '###', '', 'left', '', 0, 'id="evequantidadedias" onblur="MouseBlur(this);"'); 
                ?> 
            </td> 
        </tr>	
        <tr>
            <td class ="SubTituloDireita" align="right">Período do Evento: </td> 
            <td>  
                <?php
                $evedatainicio = $rsDadosEvento[0]["evedatainicio"];
                $evedatafim = $rsDadosEvento[0]["evedatafim"];
                ?>
                <?= campo_data('evedatainicio', 'S', 'S', '', 'S'); ?> à  
                <?= campo_data('evedatafim', 'S', 'S', '', 'S'); ?> 
            </td> 
        </tr>
        <!-- 
        <tr>
            <td class ="SubTituloDireita" align="right">Local do Evento: </td>
            <td> 
                <? // $evelocal  = $rsDadosEvento[0]["evelocal"];  ?>     
                <? //= campo_texto('evelocal', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="evelocal" onblur="MouseBlur(this);"' );  ?>
            </td>
        </tr>
        -->
        <tr>
            <td class ="SubTituloDireita" align="right">Público: </td>
            <td> 
                <? $evepublicoestimado = $rsDadosEvento[0]["evepublicoestimado"]; ?>     
                <?= campo_texto('evepublicoestimado', 'S', $somenteLeitura, 'Quantidade de Público Estimado', 8, 6, '######', '', 'left', '', 0, 'id="evepublicoestimado" onblur="MouseBlur(this);"'); ?> 
            </td> 
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">UF: </td>
            <td>  
                <?php
                $estuf = $rsDadosEvento[0]["estuf"];
                $_SESSION['uf'] = $estuf;
                $sql = "SELECT estuf as codigo, estdescricao as descricao from territorios.estado order by estdescricao";
                ?>
                <?= $db->monta_combo('estuf', $sql, $somenteLeituraUnidade, "Selecione...", 'filtraMunicipio', '', '', '100', 'S', 'estuf'); ?> 
            </td> 
        </tr>
        <tr>
            <td  class ="SubTituloDireita" align="right">Município: </td>
            <td id="municipio" name="municipio">  
                <?php
                $muncod = $rsDadosEvento[0]["muncod"];

                $sql = "select
                        muncod as codigo, 
                        mundescricao as descricao 
                       from
                        territorios.municipio 
                       order by
                        mundescricao asc";

                $db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'muncod');
                ?> 
            </td> 
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Custo(R$): </td>
            <td> 
                <?
                $evecustoprevisto = number_format($rsDadosEvento[0]["evecustoprevisto"], 2, ",", ".");
                ?>         
                <?= campo_texto('evecustoprevisto', 'N', 'N', '', 20, 20, '#.###.###.###,##', '', 'left', '', 0, 'id="evecustoprevisto" onblur="MouseBlur(this);"'); ?>

            </td>
        </tr>
        <tr>

            <td class ="SubTituloDireita" align="right">Nº Processo: </td>
            <td> 
                <?
                $evenumeroprocesso = $rsDadosEvento[0]["evenumeroprocesso"];
                if ($evenumeroprocesso) {
                    $evenumeroprocesso = str_replace("-", "", str_replace("/", "", str_replace(".", "", $evenumeroprocesso)));
                }
                $evenumeroprocesso = mascaraglobal2($evenumeroprocesso, '#####.######/####-##');
                echo campo_texto('evenumeroprocesso', 'S', $ativo, '', 25, 20, '#####.######/####-##', '', 'left', '', 0, 'id="evenumeroprocesso" ', '', '', "validaMascaraProcesso(this.value)"); 
                ?>
            </td>

        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Fiscal do Evento: </td>
            <td> 
                <? $everespnome = $rsDadosEvento[0]["everespnome"]; ?>     
                <?= campo_texto('everespnome', 'S', $somenteLeitura, '', 80, 255, '', '', 'left', '', 0, 'id="everespnome" onblur="MouseBlur(this);"'); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Telefone: </td>
            <td> 
                <? $everesptelefone = $rsDadosEvento[0]["everesptelefone"]; ?>     
                <?= campo_texto('everesptelefone', 'S', $somenteLeitura, '', 20, 25, '', '', 'left', '', 0, 'id="everesptelefone" onblur="MouseBlur(this);"'); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">E-mail: </td>
            <td> 
                <? $eveemail = $rsDadosEvento[0]["eveemail"]; ?>     
                <?= campo_texto('eveemail', 'S', $somenteLeitura, '', 40, 150, '', '', 'left', '', 0, 'id="eveemail" onblur="MouseBlur(this);"'); ?>
            </td>

        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Passagens Aéreas: </td>
            <td id="tipoValor">  
                <?
                if ($rsDadosEvento[0]['eveqtdpassagemaerea'] > 0) {
                    $selected_s = "checked = checked";
                    $selected_n = "";
                    $content = "- Quantidade: <input type='text' value='" . $rsDadosEvento[0]['eveqtdpassagemaerea'] . "' size='4' onkeyup=\"this.value=mascaraglobal('#####',this.value);\" name='eveqtdpassagemaerea' id='eveqtdpassagemaerea'><img src='../imagens/obrig.gif' title='Indica campo obrigatório.' border='0'></img>";
                } else {
                    $selected_n = "checked = checked";
                    $selected_s = "";
                    $content = "";
                }
                ?>
                <input type="radio" class="radio" name="passagens" id="passagens" value="t" <? echo $selected_s; ?> onclick="setPassagem('t');" />Sim <label for="lb_sim" name="lb_sim" id="lb_sim"> <?= $content ?> </label> 
                <br>
                <input type="radio" class="radio" name="passagens" id="passagens" value="f" <? echo $selected_n; ?> onclick="setPassagem('f');"/>Não 
            </td> 
        </tr> 
             
    <?php
    $endereco = new Endereco($rsDadosEvento[0]['endid']);
	$entidade->enderecos[0] = $endereco;
	?>	
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Coordenadas Geográficas</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Latitude</td>
		<td><?php 
		$medlatitude = $endereco->medlatitude;
		$latitude = explode(".", $medlatitude);
		$graulatitude = $latitude[0];
		$minlatitude = $latitude[1];
		$seglatitude = $latitude[2];
		$pololatitude = $latitude[3];
			
		?> <?= campo_texto( 'graulatitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="graulatitude"'); ?>
		° <?= campo_texto( 'minlatitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="minlatitude" '); ?>
		' <?= campo_texto( 'seglatitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="seglatitude" '); ?>
		'' <select name="pololatitude" id="pololatitude" class="CampoEstilo"
			style="width: 40px;">
			<option value="S"
			<?php if ($pololatitude == "S") {echo 'selected="selected"';} ?>>S</option>
			<option value="N"
			<?php if ($pololatitude == "N") {echo 'selected="selected"';} ?>>N</option>
		</select>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Longitude</td>
		<td><?php 
		$medlongitude = $endereco->medlongitude;
		$longitude = explode(".", $medlongitude);
		$graulongitude = $longitude[0];
		$minlongitude = $longitude[1];
		$seglongitude = $longitude[2];
		?> <?= campo_texto( 'graulongitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="graulongitude"'); ?>
		° <?= campo_texto( 'minlongitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="minlongitude"'); ?>
		' <?= campo_texto( 'seglongitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="seglongitude"');  ?>
		'' &nbsp; W  
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">&nbsp;</td>
		<td><a href="#" onclick="abreMapa();">Visualizar / Buscar No Mapa</a> <input style="display:none;" type="text" name="endereco[endzoom]"id="endzoom" value=<? if ($entidade->enderecos[0]->endzoom ==null) echo "15"; else echo $entidade->enderecos[0]->endzoom;?>>
		</td>
	</tr>
    <? if( !temPerfilEmpresa() ){ ?> 
    <tr>
		<td  class ="SubTituloDireita" align="right">&nbsp;</td>
		<td>
			
			<input type="hidden" name="action" value="0" id="action">
			<input type="hidden" name="eveid" value="<?=$_SESSION['eveid']?>" id="eveid">
			<input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="salvaEvento('grava');">  
			<input type="button" name="btnCancel" value="Cancelar" id="btnCancel" onclick="salvaEvento('cancel');">  
		</td>            	   
    </tr>	
    <? } ?>
		 
</table> 
</form> 
<div id="erro"></div>

<?php 
//verifica permissao de edição
echo eventoPermissaoEdicao();
?>
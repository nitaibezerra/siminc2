<?PHP

if($_REQUEST['requisicao']){
    $_REQUEST['requisicao']($_REQUEST);
}

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

    if(in_array(PERFIL_EMPRESA, $perfis) && count($perfis) == 1){
        echo "<script>document.location.href = 'evento.php?modulo=principal/cadEventoAnexo&acao=A'</script>";
    }

    if( $_SESSION['evento']['eveid'] != ''){
        $docid = evtCriarDoc( $_SESSION['evento']['eveid'] );
    }

    if( $_POST['ajaxunsetsession'] == '1'){
        $_SESSION['evento']['eveid'] = '';
        die();
    }

    include_once APPRAIZ . "includes/classes/dateTime.inc";

    if( $_POST['action'] == 'grava' ){
        if( $_SESSION['evento']['eveid'] ){
            salvar( $_SESSION['evento']['eveid'] );
        } else {
            salvar();
        }
    }

    if( $_POST['ajaxestuf'] ){
        header('content-type: text/html; charset=ISO-8859-1');

        $sql = "
            SELECT  muncod as codigo,
                    mundescricao as descricao
            FROM territorios.municipio

            WHERE estuf = '".$_POST['ajaxestuf']."'

            ORDER BY mundescricao asc
        ";
        die( $db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', '400', 'S', 'muncod') );
    }

    #BUSCA DADOS CO EVENTO NECESSARIO PARA O FUNCIONAMENTO DA TELA.
    if( $_SESSION['evento']['eveid'] != ''){

        $eveid = $_SESSION['evento']['eveid'];
        
        $rsDadosEvento = buscaDadosCadEvento( $eveid );

        $_SESSION['evento']['evedatainclusao']  = $rsDadosEvento['evedatainclusao'];
        $_SESSION['evento']['publico']          = $rsDadosEvento['evepublicoestimado'];
        $_SESSION['evento']['estuf']            = $rsDadosEvento['estuf'];
        $_SESSION['evento']['emuid']            = $rsDadosEvento['emuid'];
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

        $evedatainicio = formata_data_sql( $_REQUEST['evedatainicio'] );
        $evedatafim = formata_data_sql( $_REQUEST['evedatafim'] );

        if ($_POST['eveqtdpassagemaerea'] == '') {
            $eveqtdpassagemaerea = 0;
        } else {
            $eveqtdpassagemaerea = $_POST['eveqtdpassagemaerea'];
        }

        $evenumeroprocesso = strlen($_POST['evenumeroprocesso']) != 20 ? "null" : "'".$_POST['evenumeroprocesso']."'";

        if ($id == NULL) {
            $sqlEnd = "
                INSERT INTO
                    entidade.endereco (
                        endcep, endlog, endcom, endbai, muncod, estuf, endnum, endstatus, medlatitude, medlongitude
                    )VALUES (
                        '$endcep', '$endlog', '$endcom', '$endbai', '$muncod', '$estuf', '$endnum', '$endstatus', '$medlatitude', '$medlongitude'
                    ) RETURNING endid;
            ";
            $endid = $db->pegaUm($sqlEnd);
            $db->commit();

            $sql = "
                INSERT INTO
                    evento.evento(
                        emuid, ungcod, evetitulo, tpeid, evedatainicio, evedatafim, eveemail, everespnome, everesptelefone, evequantidadedias, evepublicoestimado, 
                        eveqtdpassagemaerea, evedatainclusao, muncod, estuf, sevid, evestatus, usucpf, endid, evenumeroprocesso, evedemandante, evecargodemandante
                    )VALUES(
                        {$_POST['emuid']}, {$_POST['ungcod']}, '{$_POST['evetitulo']}', '{$_POST['tpeid']}', '{$evedatainicio}', '{$evedatafim}', '{$_POST['eveemail']}', '{$_POST['everespnome']}',
                        '{$_POST['everesptelefone']}', '{$_POST['evequantidadedias']}', '{$_POST['evepublicoestimado']}', '{$eveqtdpassagemaerea}', 'NOW();', '{$_POST['muncod']}', '{$_POST['estuf_disable']}',
                        2, 'A', '{$_SESSION['usucpf']}', '{$endid}', {$evenumeroprocesso}, '{$evedemandante}', '{$evecargodemandante}'
                )RETURNING eveid;
            ";

            if ($eveid = $db->pegaUm($sql)){
                if ( !empty($_REQUEST['unpcod'][0]) ){
                    foreach ($_REQUEST['unpcod'] as $ungcod) {
                        $sql = "INSERT INTO evento.unidadeparceira (eveid, ungcod) VALUES ('{$eveid}', '{$ungcod}')";
                        $db->executar($sql);
                    }
                }
                $db->commit();
                $_SESSION['evento']['eveid'] = $eveid;
                $docid = evtCriarDoc($eveid);
                echo"<script>alert('Evento gravado com sucesso!'); window.location.href = 'evento.php?modulo=principal/cadEvento&acao=A';</script>";
            }
        }else{
            if (!$_POST['endereco']['endid']){
                $sqlEnd = "
                    INSERT INTO
                        entidade.endereco(
                            endcep, endlog, endcom, endbai, muncod, estuf, endnum, endstatus, medlatitude, medlongitude
                        )VALUES(
                            '{$endcep}', '{$endlog}', '{$endcom}', '{$endbai}', '{$muncod}', '{$estuf}', '{$endnum}', '{$endstatus}', '{$medlatitude}', '{$medlongitude}'
                    ) RETURNING endid;
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


            #BLOCAO UNIDADE PARCEIRA - ATUALIZA DADOS UNIDADE PARCEIRA.
            $sql_up = "DELETE FROM evento.unidadeparceira WHERE eveid = {$_SESSION['evento']['eveid']};";
            $db->executar($sql_up);
            if ( !empty($_REQUEST['unpcod'][0]) ){
                foreach ($_REQUEST['unpcod'] as $ungcod) {
                    $sql = "INSERT INTO evento.unidadeparceira (eveid, ungcod) VALUES ({$_SESSION['evento']['eveid']}, '{$ungcod}');";
                    $db->executar($sql);
                }
            }
            #FIM - BLOCAO UNIDADE PARCEIRA - ATUALIZA DADOS UNIDADE PARCEIRA.

            $sql = "
                UPDATE evento.evento
                    SET emuid               = {$_POST['emuid']},
                        evetitulo           = '{$_POST['evetitulo']}',
                        tpeid 		= '{$_POST['tpeid']}',
                        evedatainicio	= '{$evedatainicio}',
                        evedatafim		= '{$evedatafim}',
                        eveemail 		= '{$_POST['eveemail']}',
                        everespnome 	= '{$_POST['everespnome']}',
                        everesptelefone 	= '{$_POST['everesptelefone']}',
                        evequantidadedias   = '{$_POST['evequantidadedias']}',
                        evepublicoestimado  = '{$_POST['evepublicoestimado']}',
                        eveqtdpassagemaerea = '{$eveqtdpassagemaerea}',
                        muncod 		= '{$_POST['muncod']}',
                        estuf 		= '{$_POST['estuf_disable']}',
                        sevid		= 2,
                        evestatus		= 'A',
                        endid		= '{$endid}',
                        evenumeroprocesso   = {$evenumeroprocesso},
                        evedemandante       = '{$evedemandante}',
                        evecargodemandante  = '{$evecargodemandante}'
                WHERE eveid = {$_SESSION['evento']['eveid']} RETURNING eveid;
            ";
            $eveid = $db->pegaUm($sql);

            if( $eveid > 0){
                $db->commit();
                $db->sucesso( 'principal/cadEvento', '', "Evento alterado com sucesso!");
            }else{
                $db->rollback();
                $db->sucesso( 'principal/cadEvento', '', "Não foi possível realizar a operação, tente novamente mais tarde!");
            }
        }
        return true;
    }
    
    function permissaoEdicao(){
	global $db;

	if($_SESSION['evento']['eveid']){
            $sql = "
                SELECT  d.esdid
                FROM evento.evento e
                INNER JOIN workflow.documento as d on d.docid = e.docid
                WHERE e.eveid = {$_SESSION['evento']['eveid']};
            ";
            $esdid = $db->pegaUm($sql);

            if($esdid != EM_CADASTRAMENTO_WF){
                return 'N';
            } else {
                return 'S';
            }
	} else {
            return 'S';
	}
    }
    
    include  APPRAIZ."includes/cabecalho.inc";

    $titulo_modulo = "Cadastro de Eventos";
    monta_titulo( $titulo_modulo, 'Solicitar Pré-agendamento de eventos.' );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script src="../includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script>
    jQuery.noConflict();

    $1_11(document).ready(function(){
        //#VERSÃO JQUERY INSTANCIADA NO CABEÇALHO
        $1_11('.chosen-select').chosen();

        $1_11('#emuid').change(function(){
            atualizaCombosAssoc( $1_11(this).val() );
            atualizaComboEstado( $1_11(this).val() );
        });
    });

    function atualizaCombosAssoc( id ){
        jQuery.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "requisicao=atualizaCombosAssoc&emuid="+id,
            success: function(resp){
                var dados = jQuery.parseJSON(resp);
                
                if( dados.vigencia == 'N' ){
                    jQuery('#btnGravar').attr('disabled', true);
                    alert('O Contrato relacioando a esse empenho esta fora da vigencia, não é possivél realizar evento por esse contrato!');
                    return false;
                }else{
                    jQuery('#btnGravar').attr('disabled', false);
                }

                jQuery.each(dados, function(index, value) {
                   jQuery('#'+index).val(value);
                });
            }
        });
    }
    
    function atualizaComboEstado( id ){
        jQuery.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "requisicao=atualizaComboEstado&emuid="+id,
            success: function(resp){
                jQuery('#combo_estado').html(resp);
            }
        });
    }

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

            var emuid               = document.getElementById('emuid');
            var datai               = document.getElementById('evedatainicio');
            var dataf               = document.getElementById('evedatafim');
            var evetitulo           = document.getElementById('evetitulo');
            var tpeid               = document.getElementById('tpeid');
            var evequantidadedias   = document.getElementById('evequantidadedias');
            var hidquantidadedias   = document.getElementById('hid_evequantidadedias');
            var eveemail            = document.getElementById('eveemail');
            var everesptelefone     = document.getElementById('everesptelefone');
            var everespnome         = document.getElementById('everespnome');
            var evepublicoestimado  = document.getElementById('evepublicoestimado');
            var muncod              = document.getElementById('muncod');
            var estuf               = document.getElementById('estuf');
            //var evelocal          = document.getElementById('evelocal');
            var evenumeroprocesso   = document.getElementById('evenumeroprocesso');

            var evedemandante       = document.getElementById('evedemandante');
            var evecargodemandante  = document.getElementById('evecargodemandante');

            if(document.getElementById('eveqtdpassagemaerea')){
                var eveqtdpassagemaerea = document.getElementById('eveqtdpassagemaerea');
            }
            var passagens = document.getElementsByName('passagens');

            var tipo;

            if( tipo == 'grava' ){
                if( emuid != null ){
                    if( emuid.value == ''){
                            alert('O Campo "Unidade de Empenho" é obrigatório!');
                            document.formulario.emuid.focus();
                            return false;
                    }
                }
                if( evetitulo.value == '')
                {
                        alert('O Campo "Título" é Obrigatório');
                        document.formulario.evetitulo.focus();
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
                if( evelocal.value == ''){
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
                document.getElementById('btnGravar').disabled = true;
                document.getElementById('action').value = tipo;
                selectAllOptions(document.getElementById('unpcod'));
                document.formulario.submit();
            }

        if( tipo == 'cancel' ){
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
        new Ajax.Request(
                'evento.php?modulo=principal/cadEvento&acao=A', {
                method: 'post',
                asynchronous: false,
                parameters: '<?= (($ureid) ? "&ureidsel=" . $ureid : "") ?>&mostraPregoes=true&ungcod='+document.formulario.ungcod.value,
                onComplete: function (res){
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

    function filtraMunicipio(estuf){
        //alert( estuf);
        td = document.getElementById('municipio');
        select = document.getElementsByName('muncod')[0];

        if(select){
            select.disabled = true;
            select.options[0].text = 'Aguarde...';
            select.options[0].selected = true;
        }

        // Faz uma requisição ajax, passando o parametro 'ordid', via POST
        var req = new Ajax.Request(
                'evento.php?modulo=principal/cadEvento&acao=A', {
                method:     'post',
                parameters: '&ajaxestuf=' + estuf,
                onComplete: function (res){
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
            if (valor.length != 20 && valor.search("/") != 12 && valor.search("/") != 17)
            {
                alert('Número de Processo inválido!');
            }
        }

    <? if ($ungcod): ?>
            document.observe("dom:loaded", function() {
                pegaPregoes();
                if (document.getElementById('btnGravar').disabled) {
                    document.getElementById('ureid').disabled = true;
                }
            });
    <? endif; ?>

</script>

<?PHP

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

if (( $rsDadosEvento['sevid'] != 1) AND ( $rsDadosEvento['sevid'] != '')) {
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
if ($_SESSION['evento']['eveid']) {
    /* RETIRANDO ABA "Estrutura Orçamentária" conforme solicitado na demanda 209333 item 8
     * if ($_SESSION['evento']['eveid']) {
        array_push($res, array("descricao" => "Estrutura Orçamentária",
            "id" => "6",
            "link" => "/evento/evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A"
                )
        );
    }
    */

    if (mostraAbaDocOS($_SESSION['evento']['eveid'])) {
        array_push($res, array("descricao" => "Ordem de Serviço",
            "id" => "7",
            "link" => "/evento/evento.php?modulo=principal/cadOrdemServico&acao=A"
                )
        );
    }

    if (mostraAbaDocPagamento($_SESSION['evento']['eveid'])) {
        array_push($res, array("descricao" => "Documento de Pagamento",
            "id" => "5",
            "link" => "/evento/evento.php?modulo=principal/cadDocPagamento&acao=A"
                )
        );
    }
}

if (( $rsDadosEvento['sevid'] != 1) AND ( $rsDadosEvento['sevid'] != '')) {
    array_push(
        $res,
        array(
            "descricao" => "Avaliação",
            "id" => "0",
            "link" => "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A"
        )
    );
}

if ($_SESSION['evento']['eveid']) {
    echo'<br><br>';
    echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=principal/cadEvento&acao=A");
}

?>

<form id="formulario" name = "formulario" action="" method="POST">
    <input type="hidden" name="eveid" id="eveid" value="<?=$_SESSION['evento']['eveid']?>"/>
    <input type="hidden" name="action" id="action" value=""/>
    <input type="hidden" name="update" id="update" value=""/>
    <input type="hidden" name="insert" id="insert" value=""/>
    <input type="hidden" name="ureid" id="ureid" value=""/>
    <input type="hidden" name="ungcod" id="ungcod" value="<?=$rsDadosEvento['ungcod'];?>"/>

    <table class="tabela listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <?php
            #BUSCA A DATA DE INCLUSAO DO EVENTO.
            $evedatainclusao = buscaDataInclusaoEvento();
            $data_nova_regra_evento = strtotime(DATA_NOVO_REGRA_EVENTO_NOVA_INFRA);

            if( $evedatainclusao >= $data_nova_regra_evento ){
        ?>
        <tr>
            <td class ="SubTituloDireita" align="right"> Empenho: </td>
            <td id="td_combo_preid">
                <?PHP
                    $edcao = permissaoEdicao();                
                    $perfis = pegaPerfilGeral();
                    $emuid  = $rsDadosEvento['emuid'];
                    
                    if( $edcao == 'S' ){
                        $empstatus = "AND e.empstatus = 'A'";
                    }else{
                        $empstatus = "";                        
                    }
                    
                    if( in_array(EVENTO_PERFIL_CADASTRADOR, $perfis) || in_array(EVENTO_PERFIL_FISCAL_EVENTO, $perfis) ){
                        $sql = "
                            SELECT  emuid AS codigo,
                                    empnumero ||' - '|| empdescricao ||' - '|| ug.ungabrev AS descricao
                            FROM evento.empenho_unidade AS e

                            JOIN evento.unidaderecurso AS u ON u.ureid = e.ureid
                            JOIN public.unidadegestora AS ug ON ug.ungcod = u.ungcod
                            JOIN evento.usuarioresponsabilidade AS us ON us.ungcod = u.ungcod

                            WHERE rpustatus = 'A' AND us.usucpf = '{$_SESSION['usucpf']}' {$empstatus}

                            ORDER BY descricao
                        ";
                    }else{
                        $sql = "
                            SELECT  emuid AS codigo,
                                    empnumero ||' - '|| empdescricao ||' - '|| ug.ungabrev AS descricao
                            FROM evento.empenho_unidade AS e
                            JOIN evento.unidaderecurso AS u ON u.ureid = e.ureid
                            JOIN public.unidadegestora AS ug ON ug.ungcod = u.ungcod
                            WHERE 1 = 1 {$empstatus}
                            ORDER BY descricao
                        ";                        
                    }
                    $db->monta_combo('emuid', $sql, 'S', "Selecione...", '', '', '', 500, 'S', 'emuid', '', $emuid, 'Empenho', '', 'chosen-select');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%"> Pregão: </td>
            <td>
                <?PHP
                    $predescpregao = $rsDadosEvento['predescpregao'];
                    echo campo_texto('predescpregao', 'S', 'N', '', 77, 200, '', '', 'left', '', 0, 'id="predescpregao"', '', $predescpregao);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%"> Contrato:</td>
            <td>
                <?PHP
                    $coenumcontrato = $rsDadosEvento['coenumcontrato'];
                    echo campo_texto('coenumcontrato', 'S', 'N', '', 77, 200, '', '', 'left', '', 0, 'id="coenumcontrato"', '', $coenumcontrato);
                ?>
            </td>
        </tr>
        <?PHP
            }
        ?>
        <tr>
            <td class ="SubTituloDireita" width="30%"> Unidade:</td>
            <td>
                <?PHP
                    $ungdsc = $rsDadosEvento['ungdsc'];
                    echo campo_texto('ungdsc', 'S', 'N', '', 77, 200, '', '', 'left', '', 0, 'id="ungdsc"', '', $ungdsc);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Título do Evento:
            </td>
            <td>
                <?PHP
                    $evetitulo = $rsDadosEvento['evetitulo'];
                    echo campo_texto('evetitulo', 'S', $somenteLeitura, '', 77, 300, '', '', 'left', '', 0, 'id="evetitulo" onblur="MouseBlur(this);"');
                ?>
            </td>
            <td rowspan="14" style="background-color:#fafafa; color:#404040; width: 1px;" valign="top" align="left">
                <?PHP
                    #Barra de estado atual e ações e Historico
                    if ($_SESSION['evento']['eveid'] != '') {
                        wf_desenhaBarraNavegacao($docid, array('' => ''));
                    }
                ?>
            </td>
        </tr>
        <!--Demandante-->
        <tr>
            <td class ="SubTituloDireita" align="right">Nome do Demandante:</td>
            <td>
                <?PHP
                    $evedemandante = $rsDadosEvento['evedemandante'];
                    echo campo_texto('evedemandante', 'S', $somenteLeitura, '', 61, 200, '', '', 'left', '', 0, 'id="evedemandante"', '', $evedemandante);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Cargo do Demandante:</td>
            <td>
                <?PHP
                    $evecargodemandante = $rsDadosEvento['evecargodemandante'];
                    echo campo_texto('evecargodemandante', 'S', $somenteLeitura, '', 61, 200, '', '', 'left', '', 0, 'id="evecargodemandante"', '', $evecargodemandante);
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Unidade(s) Parceira(s):</td>
            <td id="unParceiras">
                <?PHP
                    $sql = sprintf("SELECT
                                        ungcod AS codigo,
                                        ungabrev || ' - ' || ungdsc AS descricao
                                    FROM
                                        public.unidadegestora
                                    WHERE ungcod <> '%s'
                                    ORDER BY ungdsc", $ungcod);

                    if ($_SESSION['evento']['eveid']) {
                        $sql_seleciona = sprintf("SELECT
                                                    ug.ungcod AS codigo,
                                                    ungabrev || ' - ' || ungdsc AS descricao
                                                FROM public.unidadegestora ug
                                                INNER JOIN evento.unidadeparceira up ON up.ungcod = ug.ungcod
                                                WHERE up.eveid = '%s'
                                                ORDER BY ungdsc", $_SESSION['evento']['eveid']);
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
                <?PHP
                    $sql = "SELECT
                            tpeid AS codigo,
                            tpedescricao AS descricao
                        FROM
                            evento.tipoevento
                    ";
                    $tpeid = $rsDadosEvento["tpeid"];
                    $db->monta_combo('tpeid', $sql, 'S', "Selecione...", '', '', '', '400', 'S', 'tpeid');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Nº de dias do Evento: </td>
            <td>
                <?PHP
                    if (trim($rsDadosEvento['evequantidadedias']) != '') {
                ?>
                    <input type="hidden" name="hid_evequantidadedias" value="<?= $rsDadosEvento['evequantidadedias']; ?>" id="hid_evequantidadedias">
                <?PHP
                    } else {
                ?>
                    <input type="hidden" name="hid_evequantidadedias" value="f" id="hid_evequantidadedias">
                <?PHP
                    }
                    $evequantidadedias = $rsDadosEvento['evequantidadedias'];
                    echo campo_texto('evequantidadedias', 'S', 'S', '', 14, 3, '###', '', 'left', '', 0, 'id="evequantidadedias" onblur="MouseBlur(this);"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Período do Evento: </td>
            <td>
                <?PHP
                    $evedatainicio = $rsDadosEvento["evedatainicio"];
                    $evedatafim = $rsDadosEvento["evedatafim"];

                    echo campo_data2('evedatainicio','S','S','Período do Evento','DD/MM/YYYY','Período do Evento','', $evedatainicio, '', '', 'evedatainicio' );
                    echo ' á ';
                    echo campo_data2('evedatafim','S','S','Período do Evento','DD/MM/YYYY','Período do Evento','', $evedatafim, '', '', 'evedatafim' );
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Público: </td>
            <td>
                <?PHP
                    $evepublicoestimado = $rsDadosEvento["evepublicoestimado"];
                    echo campo_texto('evepublicoestimado', 'S', $somenteLeitura, 'Quantidade de Público Estimado', 14, 6, '######', '', 'left', '', 0, 'id="evepublicoestimado" onblur="MouseBlur(this);"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> UF: </td>
            <td id="combo_estado">
                <?PHP
                    $estuf = $rsDadosEvento["estuf"];
                    $sql = "
                        SELECT  estuf as codigo, 
                                estdescricao as descricao 
                        FROM territorios.estado 
                        ORDER BY estdescricao
                    ";
                    echo $db->monta_combo('estuf', $sql, $somenteLeituraUnidade, "Selecione...", 'filtraMunicipio', '', '', '200', 'S', 'estuf');
                ?>
            </td>
        </tr>
        <tr>
            <td  class ="SubTituloDireita" align="right"> Município: </td>
            <td id="municipio" name="municipio">
                <?PHP
                    $muncod = $rsDadosEvento["muncod"];
                    $sql = "
                        SELECT  muncod as codigo,
                                mundescricao as descricao
                        FROM territorios.municipio
                        ORDER BY mundescricao ASC
                    ";
                    $db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', '400', 'S', 'muncod');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Custo(R$): </td>
            <td>
                <?PHP
                    $evecustoprevisto = number_format($rsDadosEvento["evecustoprevisto"], 2, ",", ".");
                    echo campo_texto('evecustoprevisto', 'N', 'N', '', 27, 20, '#.###.###.###,##', '', 'left', '', 0, 'id="evecustoprevisto" onblur="MouseBlur(this);"');
                ?>
            </td>
        </tr>
        <tr>

            <td class ="SubTituloDireita" align="right">Nº Processo: </td>
            <td>
                <?PHP
                    $evenumeroprocesso = $rsDadosEvento["evenumeroprocesso"];
                    if ($evenumeroprocesso) {
                        $evenumeroprocesso = str_replace("-", "", str_replace("/", "", str_replace(".", "", $evenumeroprocesso)));
                    }
                    $evenumeroprocesso = mascaraglobal2($evenumeroprocesso, '#####.######/####-##');
                    echo campo_texto('evenumeroprocesso', 'S', $ativo, '', 27, 20, '#####.######/####-##', '', 'left', '', 0, 'id="evenumeroprocesso" ', '', '', "validaMascaraProcesso(this.value)");
                ?>
            </td>

        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Fiscal do Evento: </td>
            <td>
                <?PHP
                    $everespnome = $rsDadosEvento["everespnome"];
                    echo campo_texto('everespnome', 'S', $somenteLeitura, '', 61, 255, '', '', 'left', '', 0, 'id="everespnome" onblur="MouseBlur(this);"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Telefone: </td>
            <td>
                <?PHP
                    $everesptelefone = $rsDadosEvento["everesptelefone"];
                    echo campo_texto('everesptelefone', 'S', $somenteLeitura, '', 27, 25, '##-####-####', '', 'left', '', 0, 'id="everesptelefone" onblur="MouseBlur(this);"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">E-mail: </td>
            <td>
                <?PHP
                    $eveemail = $rsDadosEvento["eveemail"];
                    echo campo_texto('eveemail', 'S', $somenteLeitura, '', 61, 150, '', '', 'left', '', 0, 'id="eveemail" onblur="MouseBlur(this);"');
                ?>
            </td>

        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Passagens Aéreas: </td>
            <td id="tipoValor">
                <?PHP
                    if ($rsDadosEvento['eveqtdpassagemaerea'] > 0) {
                        $selected_s = "checked = checked";
                        $selected_n = "";
                        $content = "- Quantidade: <input type='text' value='" . $rsDadosEvento['eveqtdpassagemaerea'] . "' size='4' onkeyup=\"this.value=mascaraglobal('#####',this.value);\" name='eveqtdpassagemaerea' id='eveqtdpassagemaerea'><img src='../imagens/obrig.gif' title='Indica campo obrigatório.' border='0'></img>";
                    } else {
                        $selected_n = "checked = checked";
                        $selected_s = "";
                        $content = "";
                    }
                ?>
                <input type="radio" name="passagens" id="passagens" value="t" <? echo $selected_s; ?> onclick="setPassagem('t');" /> Sim
                <label for="lb_sim" name="lb_sim" id="lb_sim"> <?= $content ?> </label>&nbsp;&nbsp;
                <input type="radio" name="passagens" id="passagens" value="f" <? echo $selected_n; ?> onclick="setPassagem('f');"/> Não
            </td>
        </tr>

    <?PHP
        $endereco = new Endereco($rsDadosEvento['endid']);
	$entidade->enderecos[0] = $endereco;
    ?>
    </table>
    
    <?PHP if( !temPerfilEmpresa() ){ ?>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr class="SubTituloCentro">
                <td>
                    <input type="button" id="btnGravar" name="btnGravar" value="Gravar" onclick="salvaEvento('grava');">
                    <input type="button" id="btnCancel" name="btnCancel" value="Cancelar" onclick="salvaEvento('cancel');">
                </td>
            </tr>
        </table>
    <? } ?>
</form>

<div id="erro"></div>

<?PHP
    #verifica permissao de edição
    echo eventoPermissaoEdicao();
?>
<?php
//echo '<pre>';
//var_dump('teste');
//exit;
function EnviarArquivo($arquivo, $dados = null, $dmdid)
{
    global $db;

    if (!$arquivo || !$dmdid)
        return false;

    // obtém o arquivo
    #$arquivo = $_FILES['arquivo'];
    if (!is_uploaded_file($arquivo['tmp_name'])) {
        redirecionar($_REQUEST['modulo'], $_REQUEST['acao'], $parametros);
    }
    // BUG DO IE
    // O type do arquivo vem como image/pjpeg
    if ($arquivo["type"] == 'image/pjpeg') {
        $arquivo["type"] = 'image/jpeg';
    }
    //Insere o registro do arquivo na tabela public.arquivo
    $sql = "INSERT INTO public.arquivo 	
			(
				arqnome,
				arqextensao,
				arqdescricao,
				arqtipo,
				arqtamanho,
				arqdata,
				arqhora,
				usucpf,
				sisid
			)VALUES(
				'" . current(explode(".", $arquivo["name"])) . "',
				'" . end(explode(".", $arquivo["name"])) . "',
				'" . $dados["arqdescricao"] . "',
				'" . $arquivo["type"] . "',
				'" . $arquivo["size"] . "',
				'" . date('Y-m-d') . "',
				'" . date('H:i:s') . "',
				'" . $_SESSION["usucpf"] . "',
				" . $_SESSION["sisid"] . "
			) RETURNING arqid;";
    $arqid = $db->pegaUm($sql);

    //Insere o registro na tabela demandas.anexos
    $sql = "INSERT INTO demandas.anexos 
			(
				dmdid,
				arqid,
				anxdtinclusao,
				anxstatus
			)VALUES(
			    " . $dmdid . ",
				" . $arqid . ",
				now(),
				'A'
			);";
    $db->executar($sql);

    if (!is_dir('../../arquivos/demandas/' . floor($arqid / 1000))) {
        mkdir(APPRAIZ . '/arquivos/demandas/' . floor($arqid / 1000), 0777);
    }
    $caminho = APPRAIZ . 'arquivos/' . $_SESSION['sisdiretorio'] . '/' . floor($arqid / 1000) . '/' . $arqid;
    switch ($arquivo["type"]) {
        case 'image/jpeg':
            ini_set("memory_limit", "128M");
            list($width, $height) = getimagesize($arquivo['tmp_name']);
            $original_x = $width;
            $original_y = $height;
            // se a largura for maior que altura
            if ($original_x > $original_y) {
                $porcentagem = (100 * 640) / $original_x;
            } else {
                $porcentagem = (100 * 480) / $original_y;
            }
            $tamanho_x = $original_x * ($porcentagem / 100);
            $tamanho_y = $original_y * ($porcentagem / 100);
            $image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
            $image = imagecreatefromjpeg($arquivo['tmp_name']);
            imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
            imagejpeg($image_p, $caminho, 100);
            //Clean-up memory
            ImageDestroy($image_p);
            //Clean-up memory
            ImageDestroy($image);
            break;
        default:
            if (!move_uploaded_file($arquivo['tmp_name'], $caminho)) {
                $db->rollback();
                return false;
            }
    }


    $db->commit();
    return true;
}

function addDayIntoDate($date, $days)
{
    $thisyear = substr($date, 0, 5);
    $thismonth = substr($date, 5, 3);
    $thisday = substr($date, 8, 4);
    $nextdate = mktime(0, 0, 0, $thismonth, $thisday + $days, $thisyear);
    return strftime("%Y%m%d", $nextdate);
}

function salvaDemanda()
{
    global $db;

    $select = array();
    $dado = array();

    if ($_POST['outrouser'] == 1) {
        $select[0] = 'dmdnomedemandante';
        $dado[0] = $_POST['usudemandante'];
        $select[1] = "dmdemaildemandante";
        $dado[1] = $_POST['usermail'];
        $select[2] = "usucpfdemandante";
        if ($_POST['usucpf']) {
            $dado[2] = $_POST['usucpf'];
            $usucpf = $_POST['usucpf'];
        } else {
            $dado[2] = $_SESSION['usucpf'];
            $usucpf = $_SESSION['usucpf'];
        }
    } else {
        $select[0] = "usucpfdemandante";
        $dado[0] = $_POST['usucpf'];
        $usucpf = $_POST['usucpf'];
    }


    if ($_POST['horarioA'] && $_POST['horarioN']) {
        $horario = "T";
    } elseif ($_POST['horarioA'] && !$_POST['horarioN']) {
        $horario = "A";
    } elseif (!$_POST['horarioA'] && $_POST['horarioN']) {
        $horario = "N";
    } else {
        $horario = "C";
    }

    if (!$_POST['atendimentoRemoto'])
        $_POST['atendimentoRemoto'] = 'f';
    if (!$_POST['dmdatendurgente'])
        $_POST['dmdatendurgente'] = 'f';

    if (!$_POST['dmdqtde'])
        $_POST['dmdqtde'] = '1';

    $sql = "SELECT 
                ur.usucpfexecutor as cpf,
                u.usunome as nome
            FROM pde.usuarioresponsabilidade ur
            INNER JOIN seguranca.usuario u ON ur.usucpfexecutor = u.usucpf
            WHERE ur.rpustatus = 'A'
            AND ur.pflcod = " . PERFIL_SUPERUSUARIO . " 
            AND ur.atiid = {$_REQUEST['atiid']}";
    $arrExecutor = $db->pegaLinha($sql);
    
    if ($arrExecutor['cpf']) {

        //Gestor do projeto
        $sql_gestorProjeto = "select u.usucpf
					from seguranca.usuario u
					inner join pde.usuarioresponsabilidade ur on ur.usucpf = u.usucpf
					inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
					where ur.rpustatus = 'A' and
					                ur.pflcod = '98' and
					                atiid = " . $_REQUEST['projeto'] . "
					order by u.usucpf";
//	ver($sql_gestorProjeto,0);
        $gestorProjeto = $db->pegaUm($sql_gestorProjeto);

        
        //Pega o analista do sistema!
//        $sql_sidid = "SELECT sidid FROM pde.atividade WHERE atistatus = 'A' AND atiid = " . $_REQUEST['projeto'];
        $sql_gestor = "select
							u.usucpf as codigo,
							u.usucpf || ' - ' || u.usunome as descricao
						from seguranca.usuario u
							inner join pde.usuarioresponsabilidade ur on
								ur.usucpf = u.usucpf
							inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
						where
							ur.rpustatus = 'A' and
							ur.pflcod = '" . PERFIL_GESTOR . "' and
							atiid = {$_REQUEST['projeto']}
						order by u.usucpf";
//	ver($sql_sidid,0);
        $cpfGestor = $db->pegaUm($sql_gestor);
        
        // Se tiver Gestor
        if ($cpfGestor) {

//            $sql = "SELECT usucpf FROM demandas.usuarioresponsabilidade WHERE pflcod = 237 AND rpustatus = 'A' AND sidid = " . $_SESSION['sisid'];
//            $sql = "SELECT usucpf FROM demandas.usuarioresponsabilidade WHERE pflcod = 237";
//            $usucpfexecutor = $db->pegaUm($sql);
            
            if ($_REQUEST['tempoexecucao'] == 1) {
                $dmddatainiprevatendimento = "08:00:00";
                $dmddatafimprevatendimento = "18:00:00";
            } else if ($_REQUEST['tempoexecucao'] == 2) {
                $dmddatainiprevatendimento = "08:00:00";
                $dmddatafimprevatendimento = "12:00:00";
            } else if ($_REQUEST['tempoexecucao'] == 3) {
                $dmddatainiprevatendimento = "14:00:00";
                $dmddatafimprevatendimento = "18:00:00";
            }

            $sql = "SELECT a.atiidpai, 
			   a.esaid, 
			   a.atidatainicio as atidatainicio,
			   a.atidatafim as atidatafim,
			   (SELECT x.atidatainicio FROM pde.atividade x WHERE x.atiid = a.atiid) as dataIpai,
			   (SELECT x.atidatafim FROM pde.atividade x WHERE x.atiid = a.atiid) as dataFpai 
			   FROM pde.atividade a WHERE a.atiid = {$_REQUEST['atiid']}";
            $dadosatv = $db->pegaLinha($sql);

            if (!empty($dadosatv['atidatainicio']) && !empty($dadosatv['atidatafim'])) {
//		$sql_executor_ = "Select dmddatainiprevatendimento,dmddatafimprevatendimento from demandas.demanda  where usucpfexecutor = '".$cpfexecutor."' and dmdstatus = 'A' and dmddatafimprevatendimento BETWEEN '".$dadosatv['atidatainicio']." ".$dmddatainiprevatendimento."' AND '".$dadosatv['atidatafim']." ".$dmddatafimprevatendimento."'";
                $sql_executor_ = "select dmddatafimprevatendimento as datafim 
						from demandas.demanda  
						where usucpfexecutor = '" . $arrExecutor['cpf'] . "' 
                		and dmdstatus = 'A' 
                		and dmddatafimprevatendimento is not null
						and dmddatafimprevatendimento BETWEEN '" . $dadosatv['atidatainicio'] . " " . $dmddatainiprevatendimento . "' AND '" . $dadosatv['atidatafim'] . " " . $dmddatafimprevatendimento . "'
						ORDER BY dmddatafimprevatendimento desc";

//		ver($sql_executor_,0);
                $recurso = $db->pegaUm($sql_executor_);
//		ver($recurso,0);
                if (!empty($recurso)) {
                    $newDate = addDayIntoDate($dadosatv['atidatafim'], 1);
                    $datainicioprevisto = date("d/m/Y", strtotime($newDate));

                    die("<script>
	 			alert(\"O Recurso," . $arrExecutor['nome'] . ", não está disponível para esta data e estará a partir da data " . $datainicioprevisto . "\");
	 			history.go(-1);
	 		</script>");
                }

                if ($_REQUEST['dmdclassificacaosistema'] == 1) {
                    $tipid = 701;
                } else if ($_REQUEST['dmdclassificacaosistema'] == 2) {
                    $tipid = 908;
                } else if ($_REQUEST['dmdclassificacaosistema'] == 3) {
                    $tipid = 908;
                } else if ($_REQUEST['dmdclassificacaosistema'] == 4) {
                    $tipid = 904;
                } else if ($_REQUEST['dmdclassificacaosistema'] == 5) {
                    $tipid = 905;
                }

                $sqlAtividade = "SELECT * FROM pde.atividade WHERE atistatus = 'A' AND atiid = " . $_REQUEST['atiid'];
                $arrAtividade = $db->pegaLinha($sqlAtividade);

                $dataForm = array(
                    'usucpfdemandante'          => $_POST['usucpf'],
                    'usucpfinclusao'            => $_POST['usucpf'],
                    'tipid'                     => $tipid,
                    'sidid'                     => ($arrAtividade['sidid'])? $arrAtividade['sidid'] : 'null',
                    'dmdtitulo'                 => $arrAtividade['atidescricao'],
                    'dmddsc'                    => $arrAtividade['atidetalhamento'],
                    'dmdreproducao'             => $_POST['reproducao'],
                    'dmdstatus'                 => $arrAtividade['atistatus'],        
                    'laaid'                     => ($_POST['laaid'] ? $_POST['laaid'] : 'null'),
                    'dmdsalaatendimento'        => $_POST['dmdsalaatendimento'],
                    'unaid'                     => $_POST['unaid'],
                    'dmdqtde'                   => $_POST['dmdqtde'],
                    'dmdhorarioatendimento'     => $horario,
                    'dmdatendremoto'            => $_POST['atendimentoRemoto'],
                    'dmddatainclusao'           => date('Y-m-d H:i:s'),
                    'dmdatendurgente'           => $_POST['dmdatendurgente'],
                    'dmddatainiprevatendimento' => $dadosatv['atidatainicio'] . " " . $dmddatainiprevatendimento,
                    'dmddatafimprevatendimento' =>  $dadosatv['atidatafim'] . " " . $dmddatafimprevatendimento,
                    
                    // Prioridade vai ser cadastrada sempre como baixa.
                    'priid'                     => '1',
                    'dmdclassificacao'          => $_POST['dmdclassificacao'],
                    'dmdclassificacaosistema'   => $_POST['dmdclassificacaosistema'],
                    'usucpfexecutor'            => $arrExecutor['cpf'],
                    'atiid'                     => $_REQUEST['atiid'],
                    'celid'                     => $_POST['celid']
                );
                
                $sql = "INSERT INTO demandas.demanda
						(
                                                    usucpfdemandante         ,
                                                    usucpfinclusao           ,
                                                    tipid                    ,
                                                    sidid                    ,
                                                    dmdtitulo                ,
                                                    dmddsc                   ,
                                                    dmdreproducao            ,
                                                    dmdstatus                ,
                                                    laaid                    ,
                                                    dmdsalaatendimento       ,
                                                    unaid                    ,
                                                    dmdqtde                  ,
                                                    dmdhorarioatendimento    ,
                                                    dmdatendremoto           ,
                                                    dmddatainclusao          ,
                                                    dmdatendurgente          ,
                                                    dmddatainiprevatendimento,
                                                    dmddatafimprevatendimento,
                                                    priid                    ,
                                                    dmdclassificacao         ,
                                                    dmdclassificacaosistema  ,
                                                    usucpfexecutor           ,
                                                    atiid                    ,
                                                    celid                    
						) VALUES (
                                                    '{$dataForm['usucpfdemandante']}',
                                                    '{$dataForm['usucpfinclusao']}',
                                                    '{$dataForm['tipid']}',
                                                    {$dataForm['sidid']},
                                                    '{$dataForm['dmdtitulo']}',
                                                    '{$dataForm['dmddsc']}',
                                                    '{$dataForm['dmdreproducao']}',
                                                    '{$dataForm['dmdstatus']}',
                                                    '{$dataForm['laaid']}',
                                                    '{$dataForm['dmdsalaatendimento']}',
                                                    '{$dataForm['unaid']}',
                                                    '{$dataForm['dmdqtde']}',
                                                    '{$dataForm['dmdhorarioatendimento']}',
                                                    '{$dataForm['dmdatendremoto']}',
                                                    '{$dataForm['dmddatainclusao']}',
                                                    '{$dataForm['dmdatendurgente']}',
                                                    '{$dataForm['dmddatainiprevatendimento']}',
                                                    '{$dataForm['dmddatafimprevatendimento']}',
                                                    '{$dataForm['priid']}',
                                                    '{$dataForm['dmdclassificacao']}',
                                                    '{$dataForm['dmdclassificacaosistema']}',
                                                    '{$dataForm['usucpfexecutor']}',
                                                    {$dataForm['atiid']},
                                                    '{$dataForm['celid']}'
						) RETURNING dmdid ";
//		ver($sql,0);
		$dmdid = $db->executar($sql);
                $db->commit();
                
                return $dmdid;
            } else {
                die("<script>
	 			alert(\"Favor atribuir uma data de Início e uma data de Término.\");
	 			history.go(-1);
	 		</script>");
            }
        } else {
            die("<script>
	 			alert(\"Favor atribuir um gestor para o sistema.\");
	 			history.go(-1);
	 		</script>");
        }
    } else {
        die("<script>
	 			alert(\"Favor atribuir um recurso à atividade.\");
	 			history.go(-1);
	 		</script>");
    }
}

/*
 * Executa a função que salva a demanda
 * Retorna mensagem de sucesso e redireciona a página
 */

if ($_POST['varaux'] == 'okCad') {

    $dmdid = salvaDemanda();

    if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], '', $dmdid)) {
        $_SESSION['dmdid'] = $dmdid;
        ?>
        <script>
            alert('Demanda cadastrada com sucesso!');
            location.href = window.location.href;
            window.opener.location.reload();
            window.close();
        </script>
        <?
        die;
    } else {
        die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");
    }
}

$usucpf = $_REQUEST['usucpf'] ? $_REQUEST['usucpf'] : $_SESSION['usucpf'];
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script>

    d = document;

    function validaForm()
    {

        if (d.getElementsByName('tempoexecucao')[0].value == '') {
            d.getElementsByName('tempoexecucao')[0].focus();
            d.getElementsByName('tempoexecucao')[0].select();
            alert('O campo Tempo de Execução, é obrigatório!');
            return false;
        }

        if (d.getElementById('dmdclassificacao').value == '') {
            alert('O campo classificação da demanda, é obrigatório!');
            return false;
        }

        if (d.getElementById('dmdclassificacaosistema').value == '') {
            alert('O campo tipo da demanda para sistemas de informação, é obrigatório!');
            return false;
        }

        d.getElementsByName('varaux')[0].value = 'okCad';
        return true;

    }
</script>
<html>
    <head>
        <title>SIMEC - Exportar Dados</title>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    </head>
    <body>
        <form id="formDemanda" action="" method="post" enctype="multipart/form-data" onsubmit="return validaForm();">
            <input type="hidden" name="usucpf" id="usucpf" value="<?= $usucpf; ?>">
            <input type="hidden" name="outrouser" id="outrouser" value="0">
            <input type="hidden" name="usermail" id="usermail" value="">
            <input type="hidden" name="verificaqtd" id="verificaqtd" value="">
            <input type="hidden" name="varaux" id="varaux" value="">
            <input type="hidden" name="ordid" id="ordid" value="1">
            <input type="hidden" name="celid" id="celid" value="2">
            <input type="hidden" name="sisid" id="sisid" value="<?= $_SESSION['sisid'] ?>">
            <input type="hidden" name="atendimentoRemoto" value="f">
            <input type="hidden" name="dmdatendurgente" value="f">
            <input type="hidden" name="unaid" id="unaid" value="15">
            <input type="hidden" name="lcaid" id="lcaid" value="3">
            <input type="hidden" name="laaid" id="laaid" value="16">
            <input type="hidden" name="dmdsalaatendimento" id="dmdsalaatendimento" value="43">
            <input type="hidden" name="usdgabinete" id="usdgabinete" value="f">
            <input type="hidden" name="usdramal" id="usdramal" value="9803">
            <table id="tblFormDemanda" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td class="SubTituloDireita">Tipo da demanda para Sistemas de Informação:</td>
                    <td>
                        <select name="dmdclassificacaosistema" id="dmdclassificacaosistema" >
                            <option value="">  Selecione  </option>
<?php if ($_SESSION['sisid'] == 15): ?>					
                                <option value="4">Manutenção corretiva</option>
                                <option value="5">Manutenção evolutiva</option>
<?php else: ?>
                                <option value="1">Inicial</option>
                                <option value="2">Consultiva</option>
                                <option value="3">Investigativa</option>
                                <option value="4">Manutenção corretiva</option>
                                <option value="5">Manutenção evolutiva</option>
<?php endif; ?>				
                        </select>
<?= obrigatorio(); ?>			
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita">Classificação da demanda:</td>
                    <td>
                        <select name="dmdclassificacao" id="dmdclassificacao" >
                            <option value="">  Selecione  </option>
                            <option value="I">Incidente</option>
                            <option value="P">Resolução de problema</option>
                            <option value="M">Requisição de mudança</option>
                            <option value="S">Solicitação de Serviço</option>
                        </select>
                            <?= obrigatorio(); ?>
                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita">Tempo de Execução:</td>
                    <td>
                        <input <?php echo $_REQUEST['tempoexecucao'] == "1" ? "checked='checked'" : "" ?> type="radio" name="tempoexecucao" value="1" checked="checked" />Dia Inteiro
                        <input <?php echo $_REQUEST['tempoexecucao'] == "2" ? "checked='checked'" : "" ?> type="radio" name="tempoexecucao" value="2" />Manhã
                        <input <?php echo $_REQUEST['tempoexecucao'] == "3" ? "checked='checked'" : "" ?> type="radio" name="tempoexecucao" value="3" />Tarde
<?= obrigatorio(); ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita"></td>
                    <td colspan="3">
                        <input type="submit" class="botao" name="btalterar" value="Abrir Demanda">
                    </td>
                </tr>
            </table>
        </form>
    </body>
</html>
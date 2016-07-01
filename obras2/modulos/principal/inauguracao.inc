<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */
verificaSessao('orgao');
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int) ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];
$obra = new Obras($_SESSION['obras2']['obrid']);
$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];

if($_GET['download']){
    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $obraArquivo = new ObrasArquivos();
    $arDados = $obraArquivo->buscaDadosPorArqid($_GET['download']);
    $eschema = 'obras2';

    $file = new FilesSimec(null,null,$eschema);
    $file->getDownloadArquivo($_GET['download']);

    die('<script type="text/javascript">
        window.close();
        </script>');
}

function verificaArqidInauguracao($iobid, $arqid)
{
    global $db;
    if(is_array($arqid)) {
        foreach ($arqid as $key => $id) {
            $arquivo = new Arquivo($id);
            $arquivo->arqdescricao = $_POST['arquivo_descricao_inauguracao'][$key];
            $arquivo->salvar();
            $arquivo->commit();
        }
    }
    
    $arqid = (is_array($arqid)) ? $arqid : array();
    $sql = "
        SELECT fio.arqid
        FROM obras2.fotos_inauguracao_obra fio
        JOIN arquivo a ON a.arqid = fio.arqid
        WHERE fio.fstatus = 'A' AND fio.iobid = $iobid AND fio.tipo = 'I'
    ";
    $arquivos  = $db->carregar($sql);

    if(!empty($arquivos)) {
        foreach ($arquivos as $arquivo) {
            if (array_search($arquivo['arqid'], $arqid) === false) {
                $sql = "UPDATE obras2.fotos_inauguracao_obra SET fstatus = 'I' WHERE arqid = {$arquivo['arqid']}";
                $db->executar($sql);
                $db->commit();
            }
        }
    }
}

function verificaArqidObraConcluida($iobid, $arqid)
{
    global $db;
    if(is_array($arqid)) {
        foreach ($arqid as $key => $id) {
            $arquivo = new Arquivo($id);
            $arquivo->arqdescricao = $_POST['arquivo_descricao_obra_concluida'][$key];
            $arquivo->salvar();
            $arquivo->commit();
        }
    }

    $arqid = (is_array($arqid)) ? $arqid : array();
    $sql = "
        SELECT fio.arqid
        FROM obras2.fotos_inauguracao_obra fio
        JOIN arquivo a ON a.arqid = fio.arqid
        WHERE fio.fstatus = 'A' AND fio.iobid = $iobid AND fio.tipo = 'C'
    ";
    $arquivos  = $db->carregar($sql);

    if(!empty($arquivos)) {
        foreach ($arquivos as $arquivo) {
            if (array_search($arquivo['arqid'], $arqid) === false) {
                $sql = "UPDATE obras2.fotos_inauguracao_obra SET fstatus = 'I' WHERE arqid = {$arquivo['arqid']}";
                $db->executar($sql);
                $db->commit();
            }
        }
    }
}

function importaDadosInauguracao($obrid)
{
    global $db;
    $sql = <<<DML
        SELECT
            iobid,
            capacidade,
            investimentototal,
            dtiniciofuncionamento,
            dtprevisaoinauguracao,
            alunosmatriculados,
            statuspc,
            quantidadehabitantes,
            distancia,
            aeroportos,
            outras
        FROM obras2.inauguracao_obra
        WHERE obrid = $obrid
            AND iobstatus = 'A'
DML;
    $dados = $db->pegaLinha($sql);
    return $dados;
}

function importaFotosDadosInauguracao($iobid) {
    global $db;
    if(!$iobid) {
        return array();
    }
    $sql = <<<DML
        SELECT fio.tipo, fio.iobid, fio.arqid, arq.arqnome, arq.arqdescricao, arq.arqextensao
        FROM obras2.fotos_inauguracao_obra fio
        INNER join public.arquivo arq ON fio.arqid = arq.arqid
        WHERE fio.iobid = $iobid
            AND fio.fstatus = 'A'
            AND arq.arqstatus = 'A'
DML;
    $dados = $db->carregar($sql);
    return $dados;
}

function salvarDados($dados) {
    global $db;
    $iobid = $dados['iobid'];
    $capacidade = $dados['capacidade'] ? MoedaToBd($dados['capacidade']) : 'NULL';
    $investimentototal = $dados['investimentototal'] ? MoedaToBd($dados['investimentototal']) : 'NULL';
    $dtiniciofuncionamento = $dados['dtiniciofuncionamento'] ? "'".$dados['dtiniciofuncionamento']."'" : 'NULL';
    $dtprevisaoinauguracao = $dados['dtprevisaoinauguracao'] ? "'".$dados['dtprevisaoinauguracao']."'" : 'NULL';
    $alunosmatriculados = $dados['alunosmatriculados'] ? MoedaToBd($dados['alunosmatriculados']) : 'NULL';
    $statuspc = $dados['statuspc'];
    $quantidadehabitantes = $dados['quantidadehabitantes'] ? MoedaToBd($dados['quantidadehabitantes']) : 'NULL';
    $distancia = $dados['distancia'] ? MoedaToBd($dados['distancia']) : 'NULL';
    $aeroportos = $dados['aeroportos'];
    $outras = $dados['outras'];
    $obrid = $dados['obrid'];
    $usucpf = $_SESSION['usucpf'];
    try {
        if ($iobid) {
            $sql = <<<DML
                UPDATE obras2.inauguracao_obra
                    SET capacidade = $capacidade, investimentototal = $investimentototal, dtiniciofuncionamento = $dtiniciofuncionamento,
                        dtprevisaoinauguracao = $dtprevisaoinauguracao, alunosmatriculados = $alunosmatriculados,
                        statuspc = '$statuspc', quantidadehabitantes = $quantidadehabitantes, distancia = $distancia,
                        aeroportos = '$aeroportos', outras = '$outras'
                WHERE obrid = $obrid
                    AND iobstatus = 'A'
                    AND iobid = {$iobid};
DML;
            $db->executar($sql);
            $db->commit();
        } else {
            $sql = <<<DML
                INSERT INTO obras2.inauguracao_obra
                    (capacidade, investimentototal, dtiniciofuncionamento, dtprevisaoinauguracao, alunosmatriculados,
                    statuspc, quantidadehabitantes, distancia, aeroportos, outras, obrid, usucpf)
                VALUES ($capacidade, $investimentototal, $dtiniciofuncionamento, $dtprevisaoinauguracao,
                    $alunosmatriculados, '$statuspc', $quantidadehabitantes, $distancia, '$aeroportos', '$outras',
                    '$obrid', '$usucpf') RETURNING iobid;
DML;
            $iobid = $db->pegaUm($sql);
            $db->commit();
        }
        $arrArqid = $dados['arquivo_obra_concluida'] ? $dados['arquivo_obra_concluida'] : array();
        if ($_FILES['arquivo_obra_concluida']['name'][0] != '') {
            require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $arquivos = $_FILES;
            foreach ($arquivos['arquivo_obra_concluida']['name'] as $key => $value) {
                if(empty($value))
                    continue;

                $files =  array(
                    'name' => $arquivos['arquivo_obra_concluida']['name'][$key],
                    'type' => $arquivos['arquivo_obra_concluida']['type'][$key],
                    'tmp_name' => $arquivos['arquivo_obra_concluida']['tmp_name'][$key],
                    'error' => $arquivos['arquivo_obra_concluida']['error'][$key],
                    'size' => $arquivos['arquivo_obra_concluida']['size'][$key]
                );
                $_FILES['arquivo'] = $files;
                $file = new FilesSimec('arquivo', null, 'obras2');
                $file->setPasta('obras2');
                $file->setUpload($dados['arquivo_descricao_obra_concluida'][$key], 'arquivo', false);
                $arqid = $file->getIdArquivo();
                if ($arqid) {
                    $arrArqid[] = $arqid;
                    $sql = <<<DML
                        INSERT INTO obras2.fotos_inauguracao_obra (iobid, tipo, arqid, obrid, usucpf) VALUES ($iobid, 'C', $arqid, $obrid, '{$_SESSION['usucpf']}');
DML;
                    $db->executar($sql);
                }
            }
            $db->commit();
        }
        verificaArqidObraConcluida($iobid, $arrArqid);

        $arrArqid = $dados['arquivo_inauguracao'] ? $dados['arquivo_inauguracao'] : array();
        if ($_FILES['arquivo_inauguracao']['name'][0] != '') {
            require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $arquivos = $_FILES;
            foreach ($arquivos['arquivo_inauguracao']['name'] as $key => $value) {
                if(empty($value))
                    continue;

                $files = array(
                    'name' => $arquivos['arquivo_inauguracao']['name'][$key],
                    'type' => $arquivos['arquivo_inauguracao']['type'][$key],
                    'tmp_name' => $arquivos['arquivo_inauguracao']['tmp_name'][$key],
                    'error' => $arquivos['arquivo_inauguracao']['error'][$key],
                    'size' => $arquivos['arquivo_inauguracao']['size'][$key]
                );
                $_FILES['arquivo'] = $files;
                $file = new FilesSimec('arquivo', null, 'obras2');
                $file->setPasta('obras2');
                $file->setUpload($dados['arquivo_descricao_inauguracao'][$key], 'arquivo', false);
                $arqid = $file->getIdArquivo();

                if ($arqid) {
                    $arrArqid[] = $arqid;
                    $sql = <<<DML
                        INSERT INTO obras2.fotos_inauguracao_obra (iobid, tipo, arqid, obrid, usucpf) VALUES ($iobid, 'I', $arqid, $obrid, '{$_SESSION['usucpf']}');
DML;
                    $db->executar($sql);
                }
            }
            $db->commit();
        }
        verificaArqidInauguracao($iobid, $arrArqid);
        echo "<script> alert('Registro salvo com sucesso!'); </script>";
    } catch (Exception $ex) {
    }
}

if ($_POST['requisicao'] == 'salvar_inauguracao') {
    salvarDados($_POST);
}

$habilitado = true;
$habil = 'S';
if ($obra->obrpercentultvistoria < 80) {
    $habilitado = false;
    $habil = 'N';
}
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
if($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros,array());
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros,array());
}
print cabecalhoObra($obrid);
monta_titulo( 'Inauguração da Obra', '');

$arrDadosInauguracao = importaDadosInauguracao($obrid);
$arrDadosFotos = importaFotosDadosInauguracao($arrDadosInauguracao['iobid']);
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet">
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script>
    $(function(){
        $('#capacidade').keyup();
        $('#investimentototal').keyup();
        $('#alunosmatriculados').keyup();
        $('#quantidadehabitantes').keyup();
        $('#distancia').keyup();
        $('#adicionar_anexo_obra_concluida').click(function (e) {
            $('#table_anexos_obra_concluida').append($('<tr class="anexos anexos-base-obr-concluida"><td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_obra_concluida" href="">Excluir</a></span></td></td><td class="SubTituloEsquerda"><td class="SubTituloEsquerda"><input <?=!$habilitado ? 'disabled' : ''?> type="file" name="arquivo_obra_concluida[]" id="arquivo_obr_concluida"/></td><td class="SubTituloEsquerda"><input <?=!$habilitado ? 'disabled' : ''?> maxlength="255" size="" type="text" name="arquivo_descricao_obra_concluida[]" id="arquivo_descricao_obr_concluida"/></td></tr>').removeClass('anexos-base-obr-concluida'));
            e.preventDefault();
        });
        $('#adicionar_anexo_inauguracao').click(function (e) {
            $('#table_anexos_inauguracao').append($('<tr class="anexos anexos-base-inauguracao"><td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_inauguracao" href="">Excluir</a></span></td><td class="SubTituloEsquerda"></td><td class="SubTituloEsquerda"><input <?=!$habilitado ? 'disabled' : ''?> type="file" name="arquivo_inauguracao[]" id="arquivo_inauguracao"/></td><td class="SubTituloEsquerda"><input <?=!$habilitado ? 'disabled' : ''?> maxlength="255" size="" type="text" name="arquivo_descricao_inauguracao[]" id="arquivo_descricao_inauguracao"/></td></tr>').removeClass('anexos-base-inauguracao'));
            e.preventDefault();
        });
        $('.excluir_anexo_obra_concluida').live('click',function (e) {
            $(this).parents('tr.anexos').remove();
            e.preventDefault();
        });
        $('.excluir_anexo_inauguracao').live('click',function (e) {
            $(this).parents('tr.anexos').remove();
            e.preventDefault();
        });
    });
</script>
<form method="post" action="" name="form_inauguracao" id="form_inauguracao" enctype="multipart/form-data">
    <input type="hidden" name="requisicao" id="requisicao" value="salvar_inauguracao">
    <input type="hidden" name="obrid" id="obrid" value="<?php echo $obrid; ?>">
    <input type="hidden" name="iobid" id="iobid" value="<?php echo $arrDadosInauguracao['iobid']?>">

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subtitulodireita" width="250px">Capacidade de atendimento:</td>
            <td>
                <?php echo campo_texto('capacidade', 'N', $habil, '', 20, 20, '[.###]', '', 'right', '', 0, 'id="capacidade"','',$arrDadosInauguracao['capacidade'], '', array('width' => '30%')); ?>
            </td>
        </tr>

        <tr>
            <td class="subtitulodireita" width="250px">Investimento Total:</td>
            <td>
                <?php echo campo_texto('investimentototal', 'N', $habil, '', 20, 20, '[.###],##', '', 'right', '', 0, 'id="investimentototal"','',$arrDadosInauguracao['investimentototal'], '', array('width' => '30%')); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Início de Funcionamento:</td>
            <td>
                <?php echo campo_data2('dtiniciofuncionamento', 'N', $habil, '', 'S', '', ' ', $arrDadosInauguracao['dtiniciofuncionamento'], ''); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Previsão de Inauguração:</td>
            <td>
                <?php echo campo_data2('dtprevisaoinauguracao', 'N', $habil, '', 'S', '', ' ', $arrDadosInauguracao['dtprevisaoinauguracao'], ''); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Alunos Matriculados:</td>
            <td>
                <?php echo campo_texto('alunosmatriculados', 'N', $habil, '', 20, 20, '[.###]', '', 'right', '', 0, 'id="alunosmatriculados"','',$arrDadosInauguracao['alunosmatriculados'], '', array('width' => '30%')); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Possuem profissionais contratados:</td>
            <td>
                <input type="radio" name="statuspc" id="statuspc" <?=!$habilitado ? 'disabled' : '' ?> value="S" <?=$arrDadosInauguracao['statuspc'] && $arrDadosInauguracao['statuspc']=='S' ? 'checked' : ''?> /> Sim &nbsp;
                <input type="radio" name="statuspc" id="statuspc" <?=!$habilitado ? 'disabled' : '' ?> value="N" <?=$arrDadosInauguracao['statuspc'] && $arrDadosInauguracao['statuspc']=='S' ? '' : 'checked'?> /> Não
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Quantidade de habitantes no município:</td>
            <td>
                <?php echo campo_texto('quantidadehabitantes', 'N', $habil, '', 20, 20, '[.###]', '', 'right', '', 0, 'id="quantidadehabitantes"','',$arrDadosInauguracao['quantidadehabitantes'], '', array('width' => '30%')); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Distância entre capital e município:</td>
            <td>
                <?php echo campo_texto('distancia', 'N', $habil, '', 20, 20, '[.###]', '', 'right', '', 0, 'id="distancia"','',$arrDadosInauguracao['distancia'], '', array('width' => '30%')); ?> KM
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Aeroportos mais próximos:</td>
            <td>
                <?php echo campo_texto('aeroportos', 'N', $habil, '', 10, 250, '', '', 'left', '', 0, 'id="aeroportos"','',$arrDadosInauguracao['aeroportos'], '', array('width' => '100%')); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Outras informações:</td>
            <td>
                <?php echo campo_textarea( 'outras', 'N', $habil, '', 50, 4, $max, $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $arrDadosInauguracao['outras'], '100%', 'outras', $opcoes = null); ?>
            </td>
        </tr>
    </table>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr bgcolor="">
            <td class="subtitulocentro" colspan="2">Fotos</td>
        </tr>
        <tr>
            <td class="subtitulocentro">Obra Concluída</td>
            <td class="subtitulocentro">Inauguração</td>
        </tr>
        <tr>
            <td width="50%">
                <div>
                    <table id="table_anexos_obra_concluida" align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                        <tr>
                            <td class="" style="width:64px;">
                                <div style="width:64px;"><img src="/imagens/gif_inclui.gif" alt=""/> <a href="" id="adicionar_anexo_obra_concluida">Adicionar</a></div>
                            </td>
                            <td>Pré-visualizaçao</td>
                            <td>Nome</td>
                            <td>Descrição</td>
                        </tr>
                        <?
                        if (!empty($arrDadosFotos)):
                            foreach ($arrDadosFotos as $arquivo): if($arquivo['tipo'] == 'I') continue;?>
                                <tr class="anexos">
                                    <td class="SubTituloEsquerda" style="width: 56px;">
                                        <span style=""><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_obra_concluida" href="">Excluir</a></span>
                                    </td>
                                    <td class="SubTituloEsquerda">
                                        <img style="width:95px;" src='data:image/png;base64,<?=base64_encode(file_get_contents(APPRAIZ."arquivos/obras2/".floor($arquivo['arqid']/1000)."/".$arquivo['arqid']))?>' style="width:97px; height:97px;" alt=""/>
                                    </td>
                                    <td class="SubTituloEsquerda">
                                        <input type="hidden" value="<?=$arquivo['arqid']?>" name="arquivo_obra_concluida[]" id="arquivo_obr_concluida"/>
                                        <a target="_blank" href="/obras2/obras2.php?modulo=principal/inauguracao&acao=A&download=<?=$arquivo['arqid']?>">
                                            <?=$arquivo['arqnome']?>.<?=$arquivo['arqextensao']?>
                                        </a>
                                    </td>
                                    <td class="SubTituloEsquerda"><input type="text" maxlength="255" size="" value="<?=$arquivo['arqdescricao']?>" name="arquivo_descricao_obra_concluida[]" id="arquivo_descricao_obr_concluida"/></td>
                                </tr>
                            <?
                            endforeach;
                        endif;
                        ?>
                        <tr class="anexos">
                            <td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_obra_concluida" href="">Excluir</a></span></td>
                            <td class="SubTituloEsquerda"></td>
                            <td class="SubTituloEsquerda"><input type="file" <?=!$habilitado ? 'disabled' : ''?> name="arquivo_obra_concluida[]" id="arquivo_obr_concluida"/></td>
                            <td class="SubTituloEsquerda"><input maxlength="255" <?=!$habilitado ? 'disabled' : ''?> type="text" name="arquivo_descricao_obra_concluida[]" id="arquivo_descricao_obr_concluida"/></td>
                        </tr>
                    </table>
                </div>
            </td>
            <td>
                <div>
                    <table id="table_anexos_inauguracao" align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                        <tr>
                            <td class="" style="width:64px;">
                                <div style="width:64px;"><img src="/imagens/gif_inclui.gif" alt=""/> <a href="" id="adicionar_anexo_inauguracao">Adicionar</a></div>
                            </td>
                            <td>Pré-visualizaçao</td>
                            <td>Nome</td>
                            <td>Descrição</td>
                        </tr>
                        <?
                        if (!empty($arrDadosFotos)):
                            foreach ($arrDadosFotos as $arquivo): if($arquivo['tipo'] == 'C') continue;?>
                                <tr class="anexos">
                                    <td class="SubTituloEsquerda" style="width: 56px;">
                                        <span style=""><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_inauguracao" href="">Excluir</a></span>
                                    </td>
                                    <td class="SubTituloEsquerda">
                                        <img style="width:95px;" src='data:image/png;base64,<?=base64_encode(file_get_contents(APPRAIZ."arquivos/obras2/".floor($arquivo['arqid']/1000)."/".$arquivo['arqid']))?>' style="width:97px; height:97px;" alt=""/>
                                    </td>
                                    <td class="SubTituloEsquerda">
                                        <input type="hidden" value="<?=$arquivo['arqid']?>" name="arquivo_inauguracao[]" id="arquivo_inauguracao"/>
                                        <a target="_blank" href="/obras2/obras2.php?modulo=principal/inauguracao&acao=A&download=<?=$arquivo['arqid']?>">
                                            <?=$arquivo['arqnome']?>.<?=$arquivo['arqextensao']?>
                                        </a>
                                    </td>
                                    <td class="SubTituloEsquerda"><input type="text" maxlength="255" size="" value="<?=$arquivo['arqdescricao']?>" name="arquivo_descricao_inauguracao[]" id="arquivo_descricao_inauguracao"/></td>
                                </tr>
                            <?
                            endforeach;
                        endif;
                        ?>
                        <tr class="anexos">
                            <td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_inauguracao" href="">Excluir</a></span></td>
                            <td class="SubTituloEsquerda"></td>
                            <td class="SubTituloEsquerda"><input type="file" name="arquivo_inauguracao[]" <?=!$habilitado ? 'disabled' : ''?> id="arquivo_inauguracao"/></td>
                            <td class="SubTituloEsquerda"><input maxlength="255" size="" type="text" <?=!$habilitado ? 'disabled' : ''?> name="arquivo_descricao_inauguracao[]" id="arquivo_descricao_inauguracao"/></td>
                        </tr>
                    </table>
                </div>
            </td>
        </tr>
        <tr bgcolor="#c0c0c0">
            <td colspan="2" style="text-align: center">
                <input type="submit" value="Salvar" id="registraMobiliario" style="cursor:pointer;" <?=!$habilitado ? 'disabled' : ''?>/>
            </td>
        </tr>
    </table>
</form>
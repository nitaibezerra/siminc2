<?
if ($_REQUEST['popup']) {
    $_REQUEST['popup']();
    die();
}

function excluir() {
    global $db;
    if ($_REQUEST['idaid'] != '') {

        $sql = "
            UPDATE academico.indicadoresacademicos 
                SET idaigc = NULL,
                    idacpcnumcursos = NULL,
                    idacpcperc = NULL
            WHERE idaid = " . $_POST['idaid'];
        $db->executar($sql);
        $db->commit();
?>
        <script>
            alert('Dados Excluídos');
            window.location.href = window.location.href;
        </script>
<?
    }
}

if ($_REQUEST['req']) {
    ob_clean();
    $_REQUEST['req']();
    die();
}

function salvar() {
    global $db;

    if (is_array($_POST['idaigc'])) {
        $sql = '';
        foreach ($_POST['idaigc'] as $idaid => $valor) {
            $_POST['idaigc'][$idaid]            = $_POST['idaigc'][$idaid] ? $_POST['idaigc'][$idaid] : 'NULL';
            $_POST['idacpcnumcursos'][$idaid]   = $_POST['idacpcnumcursos'][$idaid] ? $_POST['idacpcnumcursos'][$idaid] : 'NULL';
            $_POST['idacpcperc'][$idaid]        = $_POST['idacpcperc'][$idaid] ? $_POST['idacpcperc'][$idaid] : '0';

            $sql .= "
                UPDATE academico.indicadoresacademicos 
                    SET idaigc = {$_POST['idaigc'][$idaid]},
                        idacpcnumcursos = {$_POST['idacpcnumcursos'][$idaid]},
                        idacpcperc = '{$_POST['idacpcperc'][$idaid]}'
                WHERE idaid = $idaid; 
            ";
        }
        if ($sql != '') {
            $db->executar($sql);
            $db->commit();
        }
    }
    echo "
        <script>
            alert('Dados Salvos com sucesso!');
            window.location.href = window.location.href; 
        </script>
    ";
}

if ($_REQUEST['req']) {
    $_REQUEST['req']();
    die();
}
monta_titulo('Indicadores Acadêmicos', '');

?>
        
<style>
    .imgbot{
        cursor:pointer;
    }
</style>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
    $(document).ready(function() {
        $('.alterar').click(function() {
            var idaid = $(this).attr('id');
            if ($(this).attr('src') == '../imagens/alterar_pb.gif') {
                $(this).attr('src', '../imagens/alterar.gif');
                $(this).attr('title', 'Editar');
                $('[name*="[' + idaid + ']"]').attr('disabled', true);
                $('[name*="[' + idaid + ']"]').attr('readonly', true);
                $('[name*="[' + idaid + ']"]').removeClass('normal');
                $('[name*="[' + idaid + ']"]').addClass('disabled');
            } else {
                $(this).attr('src', '../imagens/alterar_pb.gif');
                $(this).attr('title', 'Bloquear');
                $('[name*="[' + idaid + ']"]').attr('disabled', false);
                $('[name*="[' + idaid + ']"]').attr('readonly', false);
                $('[name*="[' + idaid + ']"]').removeClass('disabled');
                $('[name*="[' + idaid + ']"]').addClass('normal');
            }
        });
        $('.documentos').click(function() {
            var id = $(this).attr('id');
            var imiid = $(this).attr('imiid');
            var janela = window.open('academico.php?modulo=principal/mpa/mpaIndicadoresAcademicos&acao=A&popup=popupMostraDocumentosMPAInd&idaid=' + id + '&imiid=' + imiid, 'altera_documento', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            janela.focus();
        });
        $('.demanda').click(function() {
            var id = $(this).attr('id');
            var janela = window.open('academico.php?modulo=principal/mpa/popupMostraDemandaMPAInd&acao=A&idaid=' + id, '', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            janela.focus();
        });
        $('.salvar').click(function() {
            $('#req').val('salvar');
            $('#formulario').submit();
        });
        $('.excluir').live('click', function() {
            $('#req').val('excluir');
            $('#idaid').val($(this).attr('id'));
            $('#formulario').submit();
        });
    });
</script>

<form id="formulario" name="formulario" method="post" action="">
    <input type="hidden" value="" name="req" id="req" />
    <input type="hidden" value="" name="idaid" id="idaid" />
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td>
                <table cellspacing="0" cellpadding="2" border="0" align="center" width="100%" class="listagem" style="color:333333;">
                    <?
                    switch ($_SESSION['academico']['orgid']) {
                        case '1':
                            $funid = ACA_ID_UNIVERSIDADE;
                            break;
                        case '2':
                            $funid = ACA_ID_ESCOLAS_TECNICAS;
                            break;
                        case '3':
                            $funid = ACA_ID_UNIDADES_VINCULADAS;
                            break;
                    }
                    $entid = $_SESSION['academico']['entid'];

                    $sql = "
                        SELECT  DISTINCT e.entnome,
                                ida.idaid,
                                e.entid,
                                mdi.imiid as imiid,
                                ( select count(amiid) from academico.arqmedidaindicadores where imiid = mdi.imiid ) AS qtd_arq,
                                idaigc,
                                idacpcnumcursos,
                                idacpcperc	
			FROM entidade.entidade e
                        INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid	
                        LEFT JOIN academico.indicadoresacademicos ida ON ida.entid = e.entid
                        LEFT JOIN academico.medidasindicadores mdi ON mdi.idaid = ida.idaid
                        LEFT JOIN academico.arqmedidaindicadores ami ON ami.amiid = mdi.imiid
                        
			WHERE e.entstatus = 'A' AND ef.funid = {$funid} AND e.entid = {$entid}
			
                        GROUP BY e.entnome, ida.idaid, e.entid, mdi.imiid, idaigc, idacpcnumcursos, idacpcperc
                        ORDER BY e.entnome	
                ";
                $campus = $db->carregar($sql);
                $cabecalho = Array('Ação', 'IGC', 'CPC-N° de Cursos Avaliados', 'Cursos com CPC Menor que 3', 'Medidas Saneadoras', 'QTD. Doc. Anexos');
                
                //ver($sql,d);
                if (is_array($campus)) {
?>
                    <tr>
                    <? foreach ($cabecalho as $cab) { ?>
                            <td align="center" valign="top" 
                                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
                                class="title">
                            <?= $cab ?>
                            </td>
                    <? } ?>
                    </tr>
                    <?
                    foreach ($campus as $camp) {
                        if ($camp['idaid'] == '') {
                            $sql = "INSERT INTO academico.indicadoresacademicos( entid ) VALUES ( {$camp['entid']} ) RETURNING idaid;";
                            $camp['idaid'] = $db->pegaUm($sql);
                            $db->commit();
                        }
                        ?>
                        <tr bgcolor="" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#ffffcc';">
                            <td valign="top" title="Ação" align="center">
                                <img border="0" src="../imagens/alterar.gif" title="Alterar" class="alterar" id="<?= $camp['idaid'] ?>">
                                <img border="0" src="../imagens/excluir.gif" title="Excluir" class="excluir" id="<?= $camp['idaid'] ?>">
                            </td>
                            <!-- <td valign="top" title="IFES"><?= $camp['entnome'] ?></td> -->
                            <td valign="top" title="IGC" align="center">
                                <?= campo_texto("idaigc[{$camp['idaid']}]", 'N', "N", '', 3, 2, '##', '', '', '', '', 'id="idaigc' . $camp['idaid'] . '"', '', $camp['idaigc']); ?>
                            </td>
                            <td valign="top" title="Ação" align="center">
                                <?= campo_texto("idacpcnumcursos[{$camp['idaid']}]", 'N', "N", '', 3, 2, '##', '', '', '', '', 'id="idacpcnumcursos' . $camp['idaid'] . '"', '', $camp['idacpcnumcursos']); ?>
                            </td>
                            <td valign="top" title="Ação" align="center">
                                <?= campo_texto("idacpcperc[{$camp['idaid']}]", 'N', "N", '', 3, 5, '###', '', '', '', '', 'id="idacpcperc' . $camp['idaid'] . '"', '', $camp['idacpcperc']); ?>% </td>
                            <td valign="top" title="Inserir Medidas Saneadoras " align="center">
                                <img border="0" id="<?= $camp['idaid'] ?>"  style="cursor:pointer" class="demanda" src="../imagens/consultar.gif"></td>
                            <td valign="top" title="Inserir Documento" align="center">
                                <?= $camp['qtd_arq'] ?>
                            </td>
                        </tr>
                        <?
                    }
                }
                ?>
                </table>
            </td>
        </tr>
        <tr>
            <td>
                <input type="button" class="salvar" style="cursor:pointer" value="Salvar">
            </td>
        </tr>    
    </table>
</form>
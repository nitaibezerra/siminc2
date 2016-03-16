<?PHP
    $comp = new AssInt();

    function atualizaCurso($request) {
        global $db;

        $sql = "
            SELECT  DISTINCT co_curso as codigo, 
                    no_curso as descricao

            FROM emec.cursos
            WHERE co_ies = {$request['co_ies']}
            ORDER BY no_curso
        ";
        if (count($db->carregar($sql)) > 0) {
            $db->monta_combo("form[co_curso]", $sql, "S", "Selecione um curso...", '', '', '', '', 'S', '', '', $co_curso);
            echo '<input type="hidden" name="form[co_ies]" value="' . $request['co_ies'] . '"/>';
        } else {
            return false;
        }
    }

    function abreArquivo($request){
        ob_clean();

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec(NULL, NULL, 'assint');
        $file->getDownloadArquivo($request['arqid']);
        
        echo "<script> window.close(); </script>";
        exit();
    }

    if ($_REQUEST['req']) {
        $_REQUEST['req']($_REQUEST);
        die();
    }

    switch ($_REQUEST['evento']) {
        case 'manter':
            if ($_POST['pecid']) {
                $paramUrl = "&pecid={$_POST['pecid']}";
            }
            $comp->manterPEC_G($_POST, $paramUrl);            
            break;
    }
    
    #MONTA CABEÇALHO
    include APPRAIZ . "includes/cabecalho.inc";

    #MONTA TÍTULO DA PÁGINA
    echo "<br>";
    $db->cria_aba($abacod_tela, $url, '');
    monta_titulo('Cadastro PEC-G', 'Assessoria Internacional');

	$pecid_ = (int)$_REQUEST['pecid'];
    if (!empty($pecid_)) {
        extract($comp->carregaPEC_G($pecid_) );
    }

// flag para botão salvar
$dis = false;
$co_ies = $comp->pegaIES();

//$super = $db->testa_superuser();
/* * * Recupera o array com os perfis do usuário ** */
$perfis = recuperaPerfil();

if (in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_ADMINISTRADOR, $perfis)) {
    $super = 1;
}
if ($_REQUEST['pecid']) {
    $co_ies_pec_g = $db->pegaUm("SELECT DISTINCT co_ies FROM assint.pecg WHERE co_ies IS NOT NULL AND pecid = " . $_REQUEST['pecid']);
}
if ($super) {
    $dis = true;
    $co_ies[0] = $co_ies_pec_g ? $co_ies_pec_g : $co_ies[0];
} elseif (count($co_ies) > 0 && is_null($_REQUEST['pecid'])) {
    $dis = true;
} else {
    if (in_array($co_ies_pec_g, $co_ies)) {
        $dis = true;
    }
}

/**
 * Verifica se o usuário tem acesso de gravação 
 */
$_REQUEST['dis'] = $dis;

require_once APPRAIZ . "www/includes/webservice/cpf.php";
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript" src="/includes/entidadesn.js"></script>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script src="/assint/js/assint.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form method="POST" enctype="multipart/form-data" name="formulario" id="formulario" >
    <input type="hidden" name="evento" id="evento" value="">
    <input type="hidden" name="pecid" value="<?= $pecid ?>">
    
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' width="30%" class="SubTituloDireita">IES:</td>
            <td>
                <?PHP
                    if ($co_ies_pec_g || $co_ies[0]) {
                        $sql = "
                            SELECT  co_ies as codigo,
                                    no_ies as descricao
                            FROM emec.ies
                            
                            WHERE co_ies = " . ($co_ies_pec_g ? $co_ies_pec_g : $co_ies[0]);
                        $resp = $db->carregar($sql);
                    }
                    
                    $sql = "
                        SELECT  co_ies as codigo, 
                                no_ies as descricao
                        FROM emec.ies
                        
                        WHERE 1=1 " . ($super ? "" : "AND co_ies in ( " . implode(",", $co_ies) . " )");
                    combo_popup('co_ies', $sql, 'IES', '400x400', 1, '', '', ($super || count($co_ies) > 1 ? 'S' : 'N'), '', '', 1, 400, $onpop = null, $onpush = null, $param_conexao = false, Array(Array('codigo' => 'no_ies', 'descricao' => 'IES')), $resp, $mostraPesquisa = true, $campo_busca_descricao = false, "atualizaCurso(this);", $intervalo = false, $arrVisivel = null, $arrOrdem = null);
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' colspan="2" class="SubTituloCentro"></td>
        </tr> 
        <tr>
            <td align='right' width="30%" class="SubTituloDireita">CPF:</td>
            <td>
                <?PHP echo campo_texto('form[peccpf]', 'S', ($dis ? 'S' : 'N'), 'CPF', 20, 14, '###.###.###-##', '', '', '', '', 'id="peccpf"', '', $peccpf); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">RNE:</td>
            <td>	
                <?PHP echo campo_texto('form[pecrne]', 'S', ($dis ? 'S' : 'N'), 'RNE', 50, 200, '', '', '', '', '', '', '', $pecrne); ?>
            </td>
        </tr> 
        <tr>
            <td class="SubTituloDireita">Nome:</td>
            <td>	
                <?PHP echo campo_texto('form[pecnome]', 'S', 'N', 'Nome', 50, 200, '', '', '', '', '', '', '', $pecnome); ?>
            </td>
        </tr>  
        <tr>
            <td align='right' class="SubTituloDireita">Sexo:</td>
            <td>
                <?PHP
                    $sql = Array(Array('codigo' => 'M', 'descricao' => 'Masculino'), Array('codigo' => 'F', 'descricao' => 'Feminino'));

                    $db->monta_combo("form[pecsexo]", $sql, ($dis ? 'S' : 'N'), "Selecione...", '', '', '', '', 'S', '', '', $pecsexo, 'Sexo');
                ?>
            </td>
        </tr>  
        <tr>
            <td class="SubTituloDireita">Matrícula:</td>
            <td>	
                <?PHP echo campo_texto('form[pecmatricula]', 'S', ($dis ? 'S' : 'N'), 'Matrícula', 50, 200, '', '', '', '', '', '', '', $pecmatricula); ?>
            </td>
        </tr>    
        <tr>
            <td align='right' class="SubTituloDireita">País de Origem:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  distinct paiid as codigo, 
                                paidescricao as descricao 
                        FROM territorios.pais 

                        ORDER BY paidescricao ASC
                    ";
                    $db->monta_combo("form[paiid]", $sql, ($dis ? 'S' : 'N'), "Selecione um País...", '', '', '', '', 'S', '', '', $paiid, 'País de Origem');
                ?>
            </td>
        </tr>  
        <tr>
            <td align='right' class="SubTituloDireita">Bolsista:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  distinct bolid as codigo, 
                                boldesc as descricao 
                        FROM assint.bolsa
                        WHERE bolstatus = 'A'
                        ORDER BY 2 ASC
                    ";
                    $db->monta_combo("form[bolid]", $sql, ($dis ? 'S' : 'N'), "Selecione uma modalidade de bolsista...", '', '', '', '', 'S', '', '', $bolid, 'Bolsista');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' colspan="2" class="SubTituloCentro"></td>
        </tr>    
        <tr>
            <td align='right' class="SubTituloDireita">Curso:</td>
            <td id="td_curso">
                <?PHP
                    if ($co_ies[0] == '' && $co_ies_pec_g == ''){
                ?>
                    <select disabled="disabled" style="width: auto" class="CampoEstilo obrigatorio" name="form[co_curso]_disable">
                        <option value="">Selecione uma IES...</option>
                    </select>
                <?PHp
                    } else {
                        $sql = "
                            SELECT  DISTINCT co_curso as codigo, 
                                    no_curso as descricao
                                    
                            FROM emec.cursos
                            
                            WHERE 1=1 " . ($co_ies != '' ? " AND co_ies = " . ($co_ies_pec_g ? $co_ies_pec_g : $co_ies[0]) : 'LIMIT 10') . "
                            ORDER BY no_curso
                        ";
                        $db->monta_combo("form[co_curso]", $sql, ($dis ? 'S' : 'N'), ($co_ies != '' ? "Selecione um curso..." : "Selecione uma IES..."), '', '', '', '', 'S', '', '', $co_curso, 'Curso');
                    ?>
                        <input type="hidden" name="form[co_ies]" value="<?= ($co_ies_pec_g ? $co_ies_pec_g : $co_ies[0]) ?>"/>
                <?PHP
                    }
                ?>
            </td>
        </tr>  
        <tr>
            <td align='right' class="SubTituloDireita">Habilitação:</td>
            <td>
                <?
                    $sql = "
                        SELECT  distinct habid as codigo, 
                                habdesc as descricao 
                        FROM assint.habilitacao
                        WHERE habstatus = 'A'
                        ORDER BY 2 ASC
                    ";
                    $db->monta_combo("form[habid]", $sql, ($dis ? 'S' : 'N'), "Selecione uma habilitação...", '', '', '', '', 'S', '', '', $habid, 'Habilitação');
                ?>
            </td>
        </tr>  
        <tr>
            <td align='right' class="SubTituloDireita">Data de ingresso:</td>
            <td>
                <?PHP echo campo_data2('form[pecdataingresso]', 'S', ($dis ? 'S' : 'N'), 'Data de ingresso', 'DD/MM/YYYY', '', '', $pecdataingresso, '', '', 'pecdataingresso') ?>
            </td>
        </tr>  
        <tr>
            <td align='right' class="SubTituloDireita">Previsão de conclusão:</td>
            <td>
                <?PHP echo campo_data2('form[pecdataprevconclusao]', 'S', ($dis ? 'S' : 'N'), 'Previsão de conclusão', 'DD/MM/YYYY', '', '', $pecdataprevconclusao, '', '', 'pecdataprevconclusao') ?>
            </td>
        </tr>  
        <tr>
            <td align='right' class="SubTituloDireita">Rendimento:</td>
            <td>
                <?PHP
                    $array = Array(0 => Array('codigo' => 0, 'descricao' => 0),
                        1 => Array('codigo' => 1, 'descricao' => 1),
                        2 => Array('codigo' => 2, 'descricao' => 2),
                        3 => Array('codigo' => 3, 'descricao' => 3),
                        4 => Array('codigo' => 4, 'descricao' => 4),
                        5 => Array('codigo' => 5, 'descricao' => 5),
                        6 => Array('codigo' => 6, 'descricao' => 6),
                        7 => Array('codigo' => 7, 'descricao' => 7),
                        8 => Array('codigo' => 8, 'descricao' => 8),
                        9 => Array('codigo' => 9, 'descricao' => 9),
                        10 => Array('codigo' => 10, 'descricao' => 10)
                    );
                    $db->monta_combo("form[pecrendimento]", $array, ($dis ? 'S' : 'N'), "...", '', '', '', '', 'S', '', '', $pecrendimento, 'Rendimento');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' colspan="2" class="SubTituloCentro"></td>
        </tr>  
        <tr>
            <td align='right' class="SubTituloDireita">Situação do Aluno:</td>
            <td>
                <?
                    $sql = "
                        SELECT  distinct siaid as codigo, 
                                siadesc as descricao 
                        FROM assint.situacaoaluno
                        WHERE siastatus = 'A'
                        ORDER BY 2 ASC
                    ";
                    $db->monta_combo("form[siaid]", $sql, ($dis ? 'S' : 'N'), "Selecione uma situação...", '', '', '', '', 'S', '', '', $siaid, 'Situação do Aluno');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Arquivos </td>
            <td td="tdAnexos">
                <?PHP if ($dis) { ?>
                    <img border="0" align="absmiddle" title="Incluir Arquivo" style="cursor: pointer" src="../imagens/gif_inclui.gif" id="addArquivo" />
                    Incluir Arquivo<br><br>
                <?PHP } ?>
                    
                <table class="tabela" style="width: 80%" align="left" border="0" cellpadding="5" cellspacing="1" id="arquivos">
                    <tr>
                        <td class="SubTituloDireita" width="50%"> <center>Descrição</center> </td>
                        <td class="SubTituloDireita"> <center>Arquivo</center> </td>
                        <td class="SubTituloDireita"> <center>Data de Inclusão</center> </td>
                        <td class="SubTituloDireita" width="5%"><center> - </center></td>
                    </tr>
                    
                    <?PHP echo $comp->recuperaArquivosPEC_G($_REQUEST); ?>
                    
                    <tr id="bordainferior">
                        <td></td>
                        <td></td>
                    </tr>
                </table>
            </td>
        </tr> 
    </table>
    
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subTituloCentro">
                <input type="button" name="btalterar" id="btalterar" value="Salvar" class="botao" <?= (($dis) ? '' : 'disabled="disabled"' ) ?> />
                <input type="button" name="btcancelar" title="Ir para Lista PEC-G" value="Voltar" onclick="voltarPaginaPrincipal();">
                
                <?PHP if ($_REQUEST['pecid']) { ?>
                    <input type="button" title="Novo PEC-G" value="Novo PEC-G" onclick="redireciona('?modulo=principal/cadPEC_G&acao=A');" class="botao" <?= (($dis) ? '' : 'disabled="disabled"' ) ?> />
                <?PHP } ?>
                    
            </td>
        </tr>      
</form>
<script type="text/javascript">

    jQuery.noConflict();

    jQuery(document).ready(function () {

        jQuery('#addArquivo').click(function () {

            var qtd = jQuery('#arquivos tr').length - 1;

            if (jQuery('#' + qtd).attr('name') != '') {
                jQuery('.linha').each(function () {
                    qtd = parseInt(jQuery(this).attr('name')) + 1;
                });
            }

            var html = '<tr class="linha" id="arq' + qtd + '" name="' + qtd + '">' +
                    '<td style="border-bottom: 1px solid #cccccc;">' +
                    '<input type="text" class="obrigatorio normal" title="Descrição do Arquivo" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" value="" maxlength="255" size="51" name="arqdsc[' + qtd + ']" style="text-align:left;">' +
                    '</td>' +
                    '<td style="border-bottom: 1px solid #cccccc;">' +
                    '<input type="file" name="arq' + qtd + '"/>' +
                    '</td>' +
                    '<td style="border-bottom: 1px solid #cccccc;"> - ' +
                    '</td>' +
                    '<td style="border-bottom: 1px solid #cccccc;">' +
                    '<center>' +
                    '<img src="../imagens/excluir.gif" title="Excluir" class="excluirarq" name="arq' + qtd + '" />' +
                    '</center>' +
                    '</td>' +
                    '</tr>'
            jQuery('#bordainferior').before(html);
        });

        jQuery('.excluirarq').live('click', function () {
            if (jQuery(this).attr('id') != '') {
                if (confirm('Deseja excluir o arquivo?')) {
                    var arq = jQuery(this).attr('name');
                    jQuery.ajax({
                        type: "POST",
                        url: window.location,
                        data: "req=excluirArquivo&anxid=" + jQuery(this).attr('id'),
                        async: false,
                        success: function (msg) {
                            if (msg == 'S') {
                                jQuery('#' + arq).remove();
                                alert('Arquivo excluido com sucesso!');
                            } else {
                                alert('Arquivo não pôde ser removido. Contate o Administrador.');
                            }
                        }
                    });
                }
            } else {
                if (confirm('Deseja excluir o arquivo?')) {
                    jQuery('#' + jQuery(this).attr('name')).remove();
                }
            }
        });

        jQuery('#peccpf').blur(function () {

            var comp = new dCPF();
            var cpf = jQuery(this).val();
            cpf = cpf.replace('.', '');
            cpf = cpf.replace('.', '');
            cpf = cpf.replace('-', '');
            comp.buscarDados(cpf);

            jQuery('[name="form[pecsexo]"]').val(comp.dados.sg_sexo_rf);

            var id = jQuery(this).attr('name');
            id = id.substring(6, id.indexOf('[]'));
            if (comp.dados.no_pessoa_rf != '') {
                jQuery('[name="form[pecnome]"]').val(comp.dados.no_pessoa_rf);
            } else {
                alert('CPF não encontrado na base da Receita Federal.');
                jQuery('[name="form[pecnome]"]').val('');
                return false;
            }
        });

        jQuery('#btalterar').click(function () {
            var erro = false;
            var msg = '';
            jQuery('.obrigatorio').each(function () {
                if (jQuery(this).val() == '') {
                    msg += jQuery(this).attr('title');
                    alert('Campo(s) obrigatório(s) não preenchido(s):\n' + msg);
                    jQuery(this).focus();
                    erro = true;
                    return false;
                }
            });
            if (erro) {
                return false;
            }
            selectAllOptions(document.getElementById('co_ies'));
            jQuery('#evento').val('manter');
            jQuery('#formulario').submit();
        });
    });

    function atualizaCurso(teste) {

        var ies = teste.value;
        if (teste.checked) {
            jQuery.ajax({
                type: "POST",
                url: window.location,
                data: "req=atualizaCurso&co_ies=" + ies,
                async: false,
                success: function (msg) {
                    if (msg) {
                        jQuery('#td_curso').html(msg);
                        return false;
                    } else {
                        alert('IES sem curso.');
                        return false;
                    }
                }
            });
            return false;
        } else {
            var html = '<select disabled="disabled" style="width: auto" class="CampoEstilo obrigatorio" name="form[co_curso]_disable">' +
                    '<option value="">Selecione uma IES...</option>' +
                    '</select>';
            jQuery('#td_curso').html(html);
            return false;
        }
        return false;
    }

    function onOffCampo(campo) {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }

    function abreArquivo(arqid) {
        window.open('assint.php?modulo=principal/cadPEC_G&acao=A&req=abreArquivo&arqid=' + arqid, 'guia', 'width=40,height=40,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');

    }
    
    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'assint.php?modulo=principal/PEC_G&acao=A';
        janela.focus();
    }
    
</script>
<?PHP
    if ($_POST['alteraAjax']) {
        die(dadosAjax($_POST['alteraAjax']));
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo '<br>';
    include_once( APPRAIZ . "gestaopessoa/classes/FtFormacaoAcademica.class.inc" );

    $db->cria_aba($abacod_tela, $url, '');
    monta_titulo('Força de Trabalho', 'Formação Acadêmica');

    function dadosAjax($id) {
        global $db;
        $sql = "SELECT ft.tfoid,ft.ffacurso,ft.ffasituacao,ft.ffanomeinstituicao,ft.ffaanoconclusao,pt.tfodsc
                FROM gestaopessoa.ftformacaoacademica as ft
                INNER JOIN public.tipoformacao AS pt ON pt.tfoid = ft.tfoid
                WHERE ft.ffaid = $id";
        $dados = $db->carregar($sql);
        if ($dados) {
            $res = $dados[0]['tfoid'] . '_' . $dados[0]['ffacurso'] . '_' . $dados[0]['ffasituacao'] . '_' . $dados[0]['ffanomeinstituicao'] . '_' . $dados[0]['ffaanoconclusao'] . '_' . $dados[0]['tfodsc'];
            return $res;
        }
    }

    $bloquearEdicao = bloqueiaEdicaoFT();

    $acad = new FtFormacaoAcademica();
    if ($_REQUEST['del'] != '') {
        $acad->excluir($_REQUEST['del']);
        $acad->commit();
        $acad->sucesso("principal/forca_trabalho/cad_formacao_academica");
    }

    if ($_POST['tfoid'] != '') {
        if ($_POST['alterar'] != '') {
            $id = $acad->pegaUm("select ffaid from gestaopessoa.ftformacaoacademica where ffaid = " . $_POST["alterar"]);
        }

        if ($id){
            $acad->carregarPorId($id);
        }

        $arDados = array(
            'fdpcpf',
            'tfoid',
            'ffacurso',
            'ffasituacao',
            'ffaanoconclusao',
            'ffanomeinstituicao'
        );
        $acad->fdpcpf = $_SESSION['fdpcpf'];
        $acad->ffaordem = 1;
        $acad->popularObjeto($arDados);
        $acad->salvar();
        $acad->commit();
        $acad->sucesso("principal/forca_trabalho/cad_formacao_academica");
    }
    echo cabecalhoPessoa($_SESSION['fdpcpf']);
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">

    function validaForm() {
        var tfoid               = document.getElementById('tfoid');
        var ffacurso            = document.getElementById('ffacurso');
        var ffanomeinstituicao  = document.getElementById('ffanomeinstituicao');
        //var ffasituacao = document.getElementById('ffasituacao');
        var ffaanoconclusao     = document.getElementById('ffaanoconclusao');


        if (tfoid.value == '') {
            alert('Campo Grau de Escolaridade é obrigatório.');
            return false;
        }
        if (ffacurso.value == '') {
            alert('Campo Curso é obrigatório.');
            return false;
        }
        if (ffanomeinstituicao.value == '') {
            alert('Campo Nome da Instituição é obrigatório.');
            return false;
        }
        if (document.formulario.ffasituacao[0].checked == false && document.formulario.ffasituacao[1].checked == false && document.formulario.ffasituacao[2].checked == false) {
            alert('Campo Situação é obrigatório.');
            return false;
        }
        if (ffaanoconclusao.value == '') {
            alert('Campo Ano de Conclusão é obrigatório.');
            return false;
        }
        document.formulario.submit();
    }
    
    function excluir(id) {
        if (confirm('Deseja realmente excluir as informações de formação?')) {
            window.location.href = 'gestaopessoa.php?modulo=principal/forca_trabalho/cad_formacao_academica&acao=A&del=' + id;
        }
    }
    
    function alterar(id) {
        var tfoid               = document.getElementById('tfoid');
        var ffacurso            = document.getElementById('ffacurso');
        var ffanomeinstituicao  = document.getElementById('ffanomeinstituicao');
        var ffaanoconclusao     = document.getElementById('ffaanoconclusao');
        var alterar             = document.getElementById('alterar');
        
        var req = new Ajax.Request(
            'gestaopessoa.php?modulo=principal/forca_trabalho/cad_formacao_academica&acao=A',{
            method: 'post',
            parameters: '&alteraAjax=' + id,
            onComplete: function (res){
                var arRes = res.responseText.split("_");

                var grau = arRes[0];
                var curso = arRes[1];
                var situacao = arRes[2];
                var nome = arRes[3];
                var ano = arRes[4];
                var labelid = arRes[5];

                tfoid.value = grau;
                ffacurso.value = curso;
                if (situacao == 'c')
                    document.formulario.ffasituacao[0].checked = true;
                if (situacao == 'e')
                    document.formulario.ffasituacao[1].checked = true;
                if (situacao == 's')
                    document.formulario.ffasituacao[2].checked = true;
                ffanomeinstituicao.value = nome;
                ffaanoconclusao.value = ano;
                alterar.value = id;

                tfoid.options[0].value = grau;
                tfoid.options[0].text = labelid;
            }
        });
    }
</script>

<form name = "formulario" action="<?=$_SERVER['REQUEST_URI'];?>" method="post" id="formulario">
    <input type ="hidden" name="alterar" id="alterar" value="">

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class ="SubTituloDireita" align="right">Grau de Escolaridade: </td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  tfoid as codigo,
                                tfodsc as descricao
                        FROM public.tipoformacao
                        WHERE tfoid NOT IN (5,6,7)
                    ";
                    $db->monta_combo('tfoid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'tfoid');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Curso: </td>
            <td>
                <?PHP
                    echo campo_texto('ffacurso', 'S', $somenteLeitura, '', 80, 600, '', '', 'left', '', 0, 'id="ffacurso" onblur="MouseBlur(this);"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Nome da Instituição: </td>
            <td>
                <?PHP
                    echo campo_texto('ffanomeinstituicao', 'S', $somenteLeitura, '', 80, 600, '', '', 'left', '', 0, 'id="ffanomeinstituicao" onblur="MouseBlur(this);"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Situação </td>
            <td>
                <input type="radio" name="ffasituacao" id="ffasituacao" <?PHP if ($dados[0]['ffasituacao'] == 'c') echo 'checked = checked'; ?> value="c"> Concluído
                <input type="radio" name="ffasituacao" id="ffasituacao" <?PHP if ($dados[0]['ffasituacao'] == 'e') echo 'checked = checked'; ?> value="e"> Em andamento
                <input type="radio" name="ffasituacao" id="ffasituacao" <?PHP if ($dados[0]['ffasituacao'] == 's') echo 'checked = checked'; ?> value="s"> Suspenso
                <?PHP echo obrigatorio(); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Ano de Conclusão: </td>
            <td>
                <?PHP
                    echo campo_texto('ffaanoconclusao', 'S', $somenteLeitura, '', 4, 5, '####', '', 'left', '', 0, 'id="ffaanoconclusao" onblur="MouseBlur(this);"'); 
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">  </td>
            <td>
                <input type="button" name="btSalvar" id="btSalvar" onclick="validaForm();" value="Salvar" <?=$bloquearEdicao;?>>
            </td>
        </tr>
    </table>
</form>

<?PHP
    $sql = "
        SELECT  '<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: alterar('||ft.ffaid ||' );\" title=\"Alterar Formação\"> &nbsp;
                <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"javascript: excluir('||ft.ffaid ||' );\" title=\"Excluir Formação\"> ' as acao,
                pt.tfodsc,
                ft.ffacurso,
                ft.ffanomeinstituicao,
                CASE 
                    WHEN UPPER( ft.ffasituacao ) = 'S' THEN 'Suspenso'
                    WHEN UPPER( ft.ffasituacao ) = 'C' THEN 'Concluído ' 
                    WHEN UPPER( ft.ffasituacao ) = 'E' THEN 'Em andamento'
		END  AS ffasituacao,
                ft.ffaanoconclusao
        FROM gestaopessoa.ftformacaoacademica AS ft
        INNER JOIN public.tipoformacao AS pt ON pt.tfoid = ft.tfoid
        
        WHERE ft.fdpcpf = '{$_SESSION['fdpcpf']}'
    ";
    $cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Grau de Escolaridade", "Curso", "Nome da Instituição", "Situação", "Ano de Conclusão");
    $db->monta_lista($sql, $cabecalho, 25, 10, 'N', '', '');
?>
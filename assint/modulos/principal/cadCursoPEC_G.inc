<?PHP
    header("Content-Type: text/html; charset=ISO-8859-1");
    
    $ano_curso = $_REQUEST['ano'];

    if ($_REQUEST['requisicao'] == 'cadastrarCurso'){
        cadastrarCurso($_POST);
        exit();
    }

    if ($_REQUEST['requisicao'] == 'alterarCurso') {
        alterarCurso($_POST);
        exit();
    }

    if ($_REQUEST['requisicao'] == 'exibirCurso') {
        exibirCurso($_POST);
        exit();
    }

    $comp = new AssInt();
    $ies = $comp->pegaIES();

    extract($_REQUEST);
    if ($ofpid) {
        $dados = recuperarCurso($ofpid);
        extract($dados);
    }

    $perfis = recuperaPerfil();
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" ></link>

<script language="JavaScript" type="text/javascript" src="../../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>

<script type="text/javascript">
    
    function salvarCurso(){
        var erro;
        var campos = '';
        var ofpid = $('#ofpid');
        var formulario = document.formulario;

        $.each($(".obrigatorio"), function(i, v){
            if( ($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox') ){
                if ( $(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }else
                if( $(this).attr('type') == 'radio' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var radio_box = $('input:radio[name='+name+']:checked');

                    if(!radio_box.val()){
                        erro = 1;
                        if(value == 'S'){
                            campos += '- ' + $(this).attr('title') + " \n";
                        }
                    }
            }else
                if( $(this).attr('type') == 'checkbox' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var check_box = $('input:checkbox[name='+name+']:checked');

                    if(!check_box.val()){
                        erro = 1;
                        if(value == 'S'){
                            campos += '- ' + $(this).attr('title') + " \n";
                        }
                    }
            }
        });

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            var co_ies = $("#co_ies").val();
            
            if( co_ies == '' ){
                selectAllOptions( formulario.co_ies );
            }
            
            if( ofpid.val() ){
                $('#requisicao').val('alterarCurso');
            }else{
                $('#requisicao').val('cadastrarCurso');
            }
            $('#formulario').submit();
        }
    }

    function exibirCurso(co_ies) {
        divCarregando();
        jQuery.ajax({
            url: 'assint.php?modulo=principal/cadCursoPEC_G&acao=A',
            data: {
                requisicao: 'exibirCurso', 
                co_ies: co_ies.value
            },
            async: false,
            type: 'POST',
            success: function(data) {
                jQuery("#divCursos").html(data);
                divCarregado();
            }
        });
    }

    function onOffCampo(campo) {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none') {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        } else {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }
    
</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
    <tr bgcolor="#CCCCCC">
        <?PHP if (in_array(PERFIL_PEC_G, $perfis)) { ?>
            <td align="center"><b>Cadastro Curso -  <?PHP echo $ano_curso . " - " . exibirNomeIES($ies[0]); ?> </b></td>
        <?PHP } else { ?>
            <td align="center"><b>Cadastro Curso - <?=$ano_curso;?></b></td>
        <?PHP } ?>
    </tr>
</table>

<form id="formulario" method="post" name="formulario" action="">
    <input type="hidden" id="ofpid" name="ofpid" value="<?=$ofpid;?>"/>
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    
    <?PHP    
        if (in_array(PERFIL_PEC_G, $perfis)) { 
    ?>
        <input type="hidden" id="co_ies" name="co_ies" value="<?PHP echo $ies[0];?>"/>
    <?PHP
        }
    ?>
        
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <?PHP if (in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_ADMINISTRADOR, $perfis)){ ?>
            <tr>
                <td align='right' width="30%" class="SubTituloDireita">IES:</td>
                <td>
                    <?PHP
                        if ($co_ies){
                            $sql = "
                                SELECT  co_ies as codigo,
                                        no_ies as descricao
                                FROM emec.ies                                
                                WHERE co_ies = {$co_ies}
                            ";
                            $resp = $db->carregar($sql);
                        }
                        
                        $sql = "
                            SELECT  co_ies AS codigo, 
                                    replace(no_ies, '\"', '') AS descricao
                            FROM emec.ies                            
                            WHERE 1 = 1
                        ";
                        combo_popup('co_ies', $sql, 'IES', '400x400', '1', '', '', 'S', '', '', 2, 400, $onpop = null, $onpush = null, $param_conexao = false, Array(Array('codigo' => 'no_ies', 'descricao' => 'IES')), $resp, $mostraPesquisa = true, $campo_busca_descricao = false, "exibirCurso(this);", $intervalo = false, $arrVisivel = null, $arrOrdem = null);
                    ?>
                </td>
            </tr>
                <?PHP } ?>
            <tr>
                <td class="subtituloDireita">Curso:</td>
                <td id="divCursos">
                    <?PHP
                        if( $ies[0] != '' ){
                            if (in_array(PERFIL_PEC_G, $perfis)) {
                                $aryWhere[] = "co_ies IN ({$ies[0]})";
                            } else {
                                if ($co_ies) {
                                    $aryWhere[] = "co_ies = {$co_ies}";
                                } else {
                                    $aryWhere[] = "co_ies = NULL";
                                }
                            }

                            $sql = "
                                SELECT  co_curso AS codigo, 
                                        no_curso AS descricao 
                                FROM emec.cursos " . (is_array($aryWhere) ? ' 

                                WHERE ' . implode(' AND ', $aryWhere) : '') . " 
                                ORDER BY no_curso
                            ";                        
                            $db->monta_combo('co_curso', $sql, 'S', 'Selecione um curso', '', '', '', 400, 'S', 'co_curso', false, NULL, 'Curso');
                        }else{
                            echo "Não a IES relacionado ao seu usuário!";
                        }
                    ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Campus:</td>
            <td>
                <?PHP 
                    echo campo_texto('ofpiescampus', 'N', 'S', 'Campus', 60, 250, '', '', '', '', '', 'id="ofpiescampus"'); 
                ?>
            </td>
        </tr>		
        <tr>
            <td class="subtituloDireita">Ano Edição do Programa:</td>
            <td>
                <?PHP
                    $aryTurno = array(
                        array('codigo' => "{$ano_curso}", 'descricao' => "{$ano_curso}")
                    );
                    $db->monta_combo('ofpano', $aryTurno, 'S', '', '', '', '', 190, 'S', 'ofpano', false, NULL, 'Ano Edição do Programa');
                ?>
            </td>
        </tr>		
        <tr>
            <td class="subtituloDireita">Ano Semestre de Ingresso:</td>
            <td>
                <?PHP
                    $aryTurno = array(
                        array('codigo' => "0", 'descricao' => "Selecione..."),
                        array('codigo' => "{$ano_curso}".".1", 'descricao' => "{$ano_curso}"."/1"),
                        array('codigo' => "{$ano_curso}".".2", 'descricao' => "{$ano_curso}"."/2")
                    );
                    $db->monta_combo('ofpanosemestre', $aryTurno, 'S', '', '', '', '', 190, 'S', 'ofpanosemestre', false, NULL, 'Ano Semestre de Ingresso');
                ?>
            </td>
        </tr>	
        <tr>
            <td class="subtituloDireita">Habilitação:</td>
            <td>
                <?PHP 
                    $sql = "
                        SELECT  DISTINCT habid as codigo,
                                habdesc as descricao 
                        FROM assint.habilitacao	
                        
                        WHERE habstatus = 'A' 
                        ORDER BY habdesc ASC
                    ";
                    $db->monta_combo("habid", $sql, 'S', "Selecione...", '', '', '', 190, 'S', 'habid', false, NULL, 'Habilitação');
                ?>
            </td>
        </tr>			
        <tr>
            <td class="subtituloDireita">Turno:</td>
            <td>
                <?PHP
                    $aryTurno = array(
                        array('codigo' => 'M', 'descricao' => 'Matutino'),
                        array('codigo' => 'V', 'descricao' => 'Vespertino'),
                        array('codigo' => 'I', 'descricao' => 'Integral')
                    );
                    $db->monta_combo('ofpturno', $aryTurno, 'S', 'Selecione...', '', '', '', 190, 'S', 'ofpturno', false, NULL, 'Turno');
                ?>		
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Nº Vagas:</td>
            <td><?PHP echo campo_texto('ofpnumvagas', 'S', 'S', 'Nº Vagas', '25', '3', '[#]', '', '', '', '', 'id="ofpnumvagas"'); ?></td>
        </tr>					
        <tr>
            <td class="subtituloDireita">Observações:</td>
            <td>
                <?PHP 
                    echo campo_textarea('ofpobservacao', 'N', 'S', '', '68', '6', '500', '', '', '', '', '', $ofpobservacao); 
                ?>
            </td>
        </tr>					
        <tr>
            <td align="center" bgcolor="#CCCCCC" colspan="2">
                <?PHP
                    if( $ies[0] != '' && $ano_curso != '2015' ){
                ?>
                    <input type="button" value="Salvar" id="btnSalvar" onclick="salvarCurso();"/>
                    <input type="button" value="Cancelar" id="btnCancelar" onclick="window.close();"/>
                <?PHP
                    }else{
                ?>
                    <input type="button" value="Salvar" id="btnSalvar" disabled="disabled"/>
                    <input type="button" value="Cancelar" id="btnCancelar" onclick="window.close();"/>
                <?PHP
                    }
                ?>
            </td>
        </tr>				
    </table>	
</form>

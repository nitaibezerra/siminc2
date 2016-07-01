<?php
    if ($_REQUEST['requisicao'] ) {
        $_REQUEST['entid'] = $_SESSION['academico']['entidcampus'];
        $teste = $_REQUEST['requisicao']($_REQUEST);
    }
?>

<html>
    <head>
        <title>Inserir Cursos</title>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
        
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script type="text/javascript" src="geral/js/academico.js"></script>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script type="text/javascript">
            
            $(document).ready(function(){
                $('#salvarCurso').click(function(){
                    var curdsc = $('#curdsc');
                    var tpcid = $('#tpcid');
                    var cpcprevisto = $('#cpcprevisto');
                    var turid = $('#turid');
                    var erro;
                    
                    if(!curdsc.val()){
                        alert('O campo "Nome do Curso" é um campo obrigatório!');
                        curdsc.focus();
                        erro = 1;
                        return false;
                    }
                    if(!tpcid.val()){
                        alert('O campo "Tipo de Curso" é um campo obrigatório!');
                        tpcid.focus();
                        erro = 1;
                        return false;
                    }
                    if(!cpcprevisto.val()){
                        alert('O campo "Previsto/Realizado" é um campo obrigatório!');
                        cpcprevisto.focus();
                        erro = 1;
                        return false;
                    }
                    if(!turid.val()){
                        alert('O campo "Previsto/Realizado" é um campo obrigatório!');
                        turid.focus();
                        erro = 1;
                        return false;
                    }                    
                    
                    if(!erro){
                        $('#requisicao').val('salvarNovoCurso');
                        $('#formulario').submit();
                    }
                });
            });
            
            function editarCurso(curid){
                $.ajax({
                    type    : "POST",
                    url     : window.location,
                    data    : "requisicao=buscarDadosEditarCurso&curid="+curid,
                    //dataType: "json",
                    asynchronous: false,
                    success: function(resp){
                        var dados = $.parseJSON(resp);
                        $('#curid').val(dados.curid);
                        $('#curdsc').val(dados.curdsc);
                        $('#tpcid').val(dados.tpcid);
                        $('#turid').val(dados.turid);
                    }
                });
            }
            
            function excluirCurso(curid){
                if(confirm('Deseja deletar este registro?')){
                    $.ajax({
                        type	: 'POST',
                        url     : window.location,
                        data	: 'requisicao=excluirCurso&curid='+curid,
                        success	: function(resp){
                            if( trim(resp) == 'ok' ){
                                alert('Operação realizada com sucesso.');
                                $('#formulario').submit();
                            }else{
                                alert('Falha ao tentar excluir o anexo!');
                            }
                        }
                    });
		}
            }
            

        </script>
    </head>
    
    <body>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
            <tr>
                <td style="font-weight: bolder; font-size: 14px; text-align:center;" >
                    Cadastro de Cursos.
                </td>
            </tr>
        </table>
        <form id="formulario" name="formulario" method="post" action="" >
            <input type="hidden" name="requisicao" id="requisicao" value="">
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td class="SubTituloDireita">Nome do Curso:</td>
                    <td>
                        <?php
                            echo campo_texto('curdsc', 'N', 'S', '', 60, 40, '', '', "", "", "", "id='curdsc'", "");
                        ?>
                    </td>
                </tr>
                <tr>
                    <td	 class="SubTituloDireita">Tipo de curso :</td>
                    <td>
                        <?
                        $sql = "
                            SELECT tpc.tpcid as codigo, tpc.tpcdsc as descricao, teu.orgid, tpc.tpcedicao
                            FROM academico.campus cam 
                            JOIN entidade.entidade e ON e.entid = cam.entid
                            JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
                            JOIN entidade.funentassoc fea ON fea.fueid = fe.fueid
                            JOIN entidade.entidade ea ON ea.entid = fea.entid
                            JOIN entidade.funcaoentidade fe1 ON fe1.entid = ea.entid
                            JOIN academico.orgaouo teu ON teu.funid = fe1.funid 		    
                            JOIN public.tipocurso tpc ON tpc.tpeid = teu.orgid 
                            WHERE cam.cmpid = '" . $_REQUEST['cmpid'] . "'
                        ";
                        $db->monta_combo('tpcid', $sql, 'S', 'Selecione', '', '', '', '220', 'S', 'tpcid', '', $_POST['tpcid']);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td	 class="SubTituloDireita">Previsto / Realizado :</td>
                    <td>
                        <?php
                            $sql = array( 
                                        array('codigo' => 'true', 'descricao' => 'Previsto'),
                                        array('codigo' => 'false', 'descricao' => 'Realizado')
                                    );
                            $db->monta_combo('cpcprevisto', $sql, 'S', 'Selecione', '', '', '', '220', 'S', 'cpcprevisto', '', $_POST['cpcprevisto']);
                        ?>
                    </td>
                    
                </tr>
                <tr>
                    <td class="SubTituloDireita" >Turno:</td>
                    <td>
                        <?php
                            $sql = "select turid as codigo, turdsc as descricao from public.turno";
                            $db->monta_combo("turid", $sql, 'S', 'Selecione', '', '', '', '220', 'S', "turid");
                        ?>
                    </td>
                </tr>
                <tr bgcolor="#C0C0C0">
                    <td colspan="2" align="center">
                        <input type="button" id="salvarCurso" name="salvarCurso" value="Salvar"/>
                    </td>
                </tr>
            </table>
        </form>
        
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr >
                <td style="width: 21%;" class="SubTituloDireita">Descrição :</td>
                <td>
                    <?php
                        echo campo_texto('n_curdscp', 'N', 'S', '', 30, 40, '', '', "", "", "", "id='n_curdscp'", "");
                    ?>
                    <input type=button value="Pesquisar" onclick="selecionatipocurso();">
                </td>
            </tr>
            <tr id="linhalistacurso">
                <td colspan="2" id="listacurso">
                    <?php
                    $entid = $_SESSION['academico']['entidcampus'];
                    
                    $sqlEdicao = "
                        <img src=\"../imagens/alterar.gif\" style=\"cursor:pointer\" id=\"img_edit_' || c.curid || '\" onclick=\"editarCurso(' || c.curid || ')\" /> 
                        <img id=\"img_delete_' || c.curid || '\" src=\"../imagens/excluir.gif\" style=\"cursor:pointer\"  onclick=\"excluirCurso(' || c.curid || ')\" />
                    ";	

                    $sql = "
                        SELECT  distinct 
                                '<div style=\"white-space: nowrap; \" >
                                    $sqlEdicao 
                                </div>' as codigo,
                                CASE WHEN cc.curid IS NOT NULL
                                    THEN '<font color=\"red\">'||c.curdsc||'</font>'
                                    ELSE c.curdsc 
                                END as curso, 
                                CASE WHEN cc.curid IS NOT NULL 
                                    THEN '<font color=\"red\">'||tur.turdsc||'</font>'
                                    ELSE tur.turdsc
                                END as turno
                        FROM public.curso c
                        Left JOIN academico.campuscurso cc ON cc.curid = c.curid
                        LEFT JOIN public.turno tur ON tur.turid = c.turid 
                        WHERE c.tpcid in ('1') AND c.entid = '". $entid ."' AND c.curstatus = 'A'  
                        ORDER BY curso                        
                    ";

                    $cabecalho = array("","Cursos", "Turno");
                    $db->monta_lista_simples( $sql, $cabecalho, 1000, 10, 'N', '100%','N');
                    
                    ?>
                </td>
            </tr>
        </table>
    </body>
</html>
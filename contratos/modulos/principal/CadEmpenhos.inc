<?php 

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        exit;
    }
    
    $titulo_modulo = "Empenhos";
    
    include  APPRAIZ."includes/cabecalho.inc";
    
    echo'<br>';
    monta_titulo( $titulo_modulo, 'Cadastro de Empenhos' );
    echo'<br>';
    
?>

<head>
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
    <meta http-equiv="Cache-Control" content="no-cache">
    
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript">

        $(document).ready(function(){

            $('.salvar').click(function(){
                $('#requisicao').val('salvarEmpenho');
                var valida = validarCampos();
                if(valida){
                    $('#formulario').submit();
                }
            });
            
            $('.voltar').click(function(){
                $('#form_pesquisa').val('listaPesquisa');
                $('#formulario').submit();
            });
            
            $('.pesquisar').click(function(){
                $('#form_pesquisa').val('listaPesquisa');
                $('#formulario').submit();
            });  
            
            $('.todos').click(function(){
                $('#form_pesquisa').val('listaPesquisa');
                $('#formulario').submit();
            });            

        });

        function validarCampos(){
            var erro;

            $.each($("input[type=text], select, textarea"), function(i,v){		
                if( ( $(v).val() == '' ) ){
                        erro = 1;
                }
            });

            if(erro === 1){
                alert("Todos os campos do formulário são obrigatorio.");
                return false;
            }else{
                return true;
            }
        }
        
        function inserirEmpenho(ungcod){
            $('#ungcod').val(ungcod);
            $('#form_pesquisa').val('empenho');
            $('#formulario').submit();
        }

        function alterar(emuid){
            jQuery.ajax({
                type: "POST",
                url: window.location,
                data: "requisicao=carregarEmpenho&emuid=" + emuid,
                async: false,
                success: function(dados) {
                    var dados = dados.split("||");
                    jQuery('#emuid').val(dados[0]);
                    jQuery('#ungcod').val(dados[1]);
                    jQuery('#empnumero').val(dados[2]);
                    jQuery('#empdescricao').val(dados[3]);
                    jQuery('#empnumeropi').val(dados[4]);
                    jQuery('#empano').val(dados[5]);
                }
            });
        }
        
        function excluir(emuid){
            $('#emuid').val(emuid);
            $('#requisicao').val('exclirEmpenho');
            $('#formulario').submit();
        }

    </script>

</head>

<?php
    if($_REQUEST['form_pesquisa'] == 'empenho'){
?>
    <form name ="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
        <input type="hidden" name="requisicao" id="requisicao" value="">
        <input type="hidden" name="form_pesquisa" id="form_pesquisa" value="">
        <input type="hidden" name="emuid" id="emuid" value="">

        <table class="listagem" width="95%" align="center" border="0" cellpadding="2" cellspacing="0">
            <tr>
                <td class ="SubTituloDireita" align="right"> Unidade Gestora:</td>
                <td>
                    <?php
                        $sql = "SELECT ungcod, ungdsc FROM public.unidadegestora WHERE ungstatus = 'A' and ungcod = '".$_REQUEST['ungcod']."'order by 1";
                        $unidade = $db->pegaLinha($sql);
                        echo campo_texto('ungdsc', 'S', 'N', '', 63, 200, '', '', 'left', '', 0, 'id="ungdsc"', '', $unidade['ungdsc']);
                    ?>
                    <input type="hidden" name="ungcod" id="ungcod" value="<?=$unidade['ungcod']?>">
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right"> Descrição do Empenho:</td>
                <td>
                    <?php echo campo_textarea('empdescricao', 'S', 'S', '', 80, 4, 200, '', 0, '', '', null, $valor); ?>
                </td>     
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right"> Código do Empenho:</td>
                <td>
                    <?php echo campo_texto('empnumero', 'S', 'S', '', 35, 15, '', '', 'left', '', 0, 'id="empnumero"'); ?>
                </td>     
            </tr>        
            <tr>
                <td class ="SubTituloDireita" align="right"> Número do PI:</td>
                <td>
                    <?php echo campo_texto('empnumeropi', 'S', 'S', '', 35, 15, '', '', 'left', '', 0, 'id="empnumeropi"'); ?>
                </td>     
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right"> Ano do PI:</td>
                <td>
                    <?php echo campo_texto('empano', 'S', 'S', '', 35, 4, '####', '', 'left', '', 0, 'id="empano"'); ?>
                </td>     
            </tr>
            <tr>
                <td class="SubTituloDireita" aling="right"></td>
                <td>
                    <input type="button" class="salvar" name="salvar" id="salvar" value="Salvar">
                    <input type="button" class="voltar" name="voltar" id="voltar" value="Voltar">
                </td>
            </tr>
        </table>
    </form>   
    <br>
<?php
        $sql = "
            SELECT  '<center>
                        <img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('||ep.emuid||')\" \" border=0 alt=\"Ir\" title=\"Alterar\">
                        <img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('||ep.emuid||');\" border=0 alt=\"Ir\" title=\"Excluir\">
                    <center>' as acoes,
                    empnumero, 
                    empdescricao, 
                    empnumeropi, 
                    empano
            FROM evento.empenho_unidade ep
            JOIN public.unidadegestora ug ON ug.ungcod = ep.ungcod
            WHERE ep.ungcod = '".$_REQUEST['ungcod']."' AND ep.empstatus = 'A'
            ORDER BY ep.emuid
        ";
        $cabecalho = array("Ações","Número do Empenho","Descrição","Número do Empenho PI","Ano do Empenho");
    $whidth = Array('5%', '10%', '40%', '10%', '10%');
    $align  = Array('left', 'left', 'left', 'left', 'left');	
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');
        
    }else{
?>
    <form name ="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <input type="hidden" name="form_pesquisa" id="form_pesquisa" value="">
    <input type="hidden" name="ungcod" id="ungcod" value="">
    
        <table class="listagem" width="95%" align="center" border="0" cellpadding="2" cellspacing="0">
            <tr>
                <td class ="SubTituloDireita" align="right"> Unidade Gestora:</td>
                <td>
                    <?php echo campo_texto('nomeuidade', 'N', 'S', '', 32, 200, '', '', 'left', '', 0, 'id="nomeuidade"'); ?>				
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right"> Código da unidade Gestora:</td>
                <td>
                    <?php echo campo_texto('codunidade', 'N', 'S', '', 32, 200, '', '', 'left', '', 0, 'id="codunidade"'); ?>
                </td>     
            </tr>
            <tr>
                <td class="SubTituloDireita" aling="right"></td>
                <td>
                    <input type="button" class="pesquisar" name="pesquisar" id="pesquisar" value="Pesquisar">  
                    <input type="button" class="todos" name="todos" id="todos" value="Ver todos">
                </td>
            </tr>
        </table>
    </form>    
    <br>
    
<?php
    $where = '';
    
    if($_REQUEST['nomeuidade']){
        $where .= "and ug.ungdsc ilike '%".$_REQUEST['nomeuidade']."%'";
    }
    
    if($_REQUEST['codunidade']){
        $where .= "and ug.ungcod = '".$_REQUEST['codunidade']."'";
    }
    
    $sql = "
        Select '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"inserirEmpenho('||ug.ungcod||')\" \" title=\"Inserir Empenho\"></center>' as acao,
                cast(ug.ungcod as text) as ungcod,
                '<a href=\"javascript:void(0);\" onclick=\"exibirExtrato(' || ur.ureid || ')\">' || ug.ungdsc || '</a>' as ungdsc,
                ureordenador,
                ureordenadorsub,
                ur.urevalorrecurso AS limite,
                coalesce(ur.urevalorsaldo,0) AS saldo
        From evento.unidaderecurso ur 
        Join public.unidadegestora ug ON ug.ungcod = ur.ungcod
        Where preid = 1 ".$where."
        Order by ug.ungdsc
    ";
    $cabecalho = array("Ações", "Cód. Unidade", "Unidades", "Ordenador de Despesa", "Ordenador de Despesa Substituto", "Limites", "Saldo"); 
    $whidth = Array('5%', '7%', '30%', '20%', '20%', '8%', '8%');
    $align  = Array('left', 'left', 'left', 'left', 'left', 'right', 'right');	
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');
    
    }
?>

<?php

$perfis = array(PFLCOD_SUPER_USUARIO);
if (!possuiPerfil($perfis)) {
    echo '<script > 
            alert(\'Você não possui permissão para acessar essa tela.\');
            window.location.href = window.location.href;
          </script>';
    die();
}

$objMonitoramento = new MonitoramentoEspecial();

switch ($_REQUEST['requisicao']) {
    case 'salvar':
        $perfis = array(PFLCOD_SUPER_USUARIO);
        if (!possuiPerfil($perfis)) {
            echo '<script > 
                    alert(\'Você não possui permissão para alterar os dados.\');
                    window.location.href = window.location.href;
                  </script>';
            die();
        }
        
        if(!empty($_REQUEST['tamdesc'])){
            $tamid = $objMonitoramento->cadastrarDadosTipoAtividade($_REQUEST);
            if(empty($tamid)){
                echo '<script > 
                        alert(\'Ocorreu um erro ao cadastrar o Tipo de Atividade de Monitoramento. Verifique se os dados foram preenchidos corretamente.\');
                        window.location.href = window.location.href;
                      </script>';
                die();
            }
        }
        
        if(!empty($_REQUEST['samdesc']) && empty($tamid)){
            $samid = $objMonitoramento->cadastrarDadosSubtipoAtividade($_REQUEST);
            if(empty($samid)){
                echo '<script > 
                        alert(\'Ocorreu um erro ao cadastrar o Subtipo de Atividade de Monitoramento. Verifique se os dados foram preenchidos corretamente.\');
                        window.location.href = window.location.href;
                      </script>';
                die();
            }
        }
        
        echo "<script > 
                alert('Dados cadastrados com sucesso.');
                window.location = '?modulo=principal/monitoramentoespecial/controleTiposAtividade&acao=A';
              </script>";
        die();
        break;
    case 'editar':
        $perfis = array(PFLCOD_SUPER_USUARIO);
        if (!possuiPerfil($perfis)) {
            echo '<script > 
                    alert(\'Você não possui permissão para alterar os dados.\');
                    window.location.href = window.location.href;
                  </script>';
            die();
        }
        
        if(!empty($_REQUEST['tamid'])){
            $tamid = $objMonitoramento->editarDadosTipoAtividade($_REQUEST);
            if(empty($tamid)){
                echo '<script > 
                        alert(\'Ocorreu um erro ao editar o Tipo de Atividade de Monitoramento. Verifique se os dados foram preenchidos corretamente.\');
                        window.location.href = window.location.href;
                      </script>';
                die();
            }
        }
        
        if(!empty($_REQUEST['samid']) && empty($tamid)){
            $samid = $objMonitoramento->editarDadosSubtipoAtividade($_REQUEST);
            if(empty($samid)){
                echo '<script > 
                        alert(\'Ocorreu um erro ao editar o Subtipo de Atividade de Monitoramento. Verifique se os dados foram preenchidos corretamente.\');
                        window.location.href = window.location.href;
                      </script>';
                die();
            }
        }
        
        echo "<script > 
                alert('Dados editados com sucesso.');
                window.location = '?modulo=principal/monitoramentoespecial/controleTiposAtividade&acao=A';
              </script>";
        die();
        break;
    case 'excluir':
        $perfis = array(PFLCOD_SUPER_USUARIO);
        if (!possuiPerfil($perfis)) {
            echo '<script > 
                    alert(\'Você não possui permissão para alterar os dados.\');
                    window.location.href = window.location.href;
                  </script>';
            die();
        }
        if(!empty($_REQUEST['tamid'])){
            $tamid = $objMonitoramento->excluirDadosTipoAtividade($_REQUEST['tamid']);
            if(empty($tamid)){
                echo '<script > 
                        alert(\'Ocorreu um erro ao excluir o Tipo de Atividade de Monitoramento. Verifique se os dados foram preenchidos corretamente.\');
                        window.location.href = window.location.href;
                      </script>';
                die();
            }
        }
        
        if(!empty($_REQUEST['samid']) && empty($tamid)){
            $samid = $objMonitoramento->excluirDadosSubtipoAtividade($_REQUEST['samid']);
            if(empty($samid)){
                echo '<script > 
                        alert(\'Ocorreu um erro ao excluir o Subtipo de Atividade de Monitoramento. Verifique se os dados foram preenchidos corretamente.\');
                        window.location.href = window.location.href;
                      </script>';
                die();
            }
        }
        
        echo "<script > 
                alert('Dado excluido com sucesso.');
                window.location = '?modulo=principal/monitoramentoespecial/controleTiposAtividade&acao=A';
              </script>";
        die();
        break;
    default :
        break;
}

include APPRAIZ . "includes/cabecalho.inc";

$arMenuBlock = array();
$id_aba = ID_ABA_MONITORAMENTO_ESPECIAL;
$db->cria_aba($id_aba, $url, $parametros, $arMenuBlock);
$titulo_modulo = 'Controle de Tipos e Subtipos de Atividade de Monitoramento';
$subtitulo_modulo = '';
monta_titulo($titulo_modulo, $subtitulo_modulo);

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" language="JavaScript">

    function alterarTipoAtividade(tamid){
        var url    = 'obras2.php?modulo=principal/monitoramentoespecial/controleTiposAtividade';
        var params = '&acao=A&tamid='+tamid;
        window.location.href = url+params;
    }
    
    function excluirTipoAtividade(tamid){
        var url    = 'obras2.php?modulo=principal/monitoramentoespecial/controleTiposAtividade';
        var params = '&acao=A&requisicao=excluir&tamid='+tamid;
        var r = confirm("Você deseja apagar o Tipo de Adividade ?");
        if (r == true) {
            window.location.href = url+params;
        }
    }
    
    function alterarSubtipoAtividade(samid){
        var url    = 'obras2.php?modulo=principal/monitoramentoespecial/controleTiposAtividade';
        var params = '&acao=A&samid='+samid;
        window.location.href = url+params;
    }
    
    function excluirSubtipoAtividade(samid){
        var url    = 'obras2.php?modulo=principal/monitoramentoespecial/controleTiposAtividade';
        var params = '&acao=A&requisicao=excluir&samid='+samid;
        var r = confirm("Você deseja apagar o Subtipo de Adividade ?");
        if (r == true) {
            window.location.href = url+params;
        }
    }
    
</script>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php 
    $titulo_modulo = 'Tipos de Atividade de Monitoramento';
    $subtitulo_modulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';
    monta_titulo($titulo_modulo, $subtitulo_modulo);
?>
<form method="post" id="formulario_tipo" name="formulario_tipo" action="" enctype="multipart/form-data">
    <input type="hidden" name="requisicao" id="requisicaoT" value=""/>
    <input type="hidden" name="tamid"      id="tamid"   value='<?php echo $_REQUEST['tamid']; ?>'/>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%" id="tabela_dadosT">
        <tr>
            <td class="subtituloDireita">Nome Tipo da Atividade:</td>
            <td colspan="2">
                <?php
                    if(!empty($_REQUEST['tamid'])){ $tamdesc = $db->pegaUm('SELECT tamdesc FROM obras2.tipoatividademonitoramento WHERE tamid = '.$_REQUEST['tamid']); }
                    $val = (!empty($tamdesc)) ? stripslashes($tamdesc) : '';
                    echo campo_texto('tamdesc', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="tamdesc"', '', $val);
                ?>
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">

            </td>
        </tr>

        <tr bgcolor="#C0C0C0" style="text-align: center">
            <td colspan="3">
                <input type='button' class='botao' name='Salvar'   value='Salvar'   id='SalvarT'   onclick="validar('formulario_tipo')"/>
                <input type='button' class='botao' name='Cancelar' value='Cancelar' id='cancelarT' onclick="window.reload()"/>
            </td>
        </tr>
    </table>
</form>
<?php 
$objMonitoramento->montaListaTipoAtividadesMonitoramento();
echo '<br><br>';
$titulo_modulo = 'SubTipos de Atividade de Monitoramento';
$subtitulo_modulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';
monta_titulo($titulo_modulo, $subtitulo_modulo);
?>
<form method="post" id="formulario_subtipo" name="formulario_subtipo" action="" enctype="multipart/form-data">
    <input type="hidden" name="requisicao" id="requisicaoS" value=""/>
    <input type="hidden" name="samid"      id="samid"   value='<?php echo $_REQUEST['samid']; ?>'/>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%" id="tabela_dadosS">
        <tr>
            <td class="subtituloDireita">Nome Subtipo da Atividade:</td>
            <td colspan="2">
                <?php
                    if(!empty($_REQUEST['samid'])){ $samdesc = $db->pegaUm('SELECT samdesc FROM obras2.subtipoatividademonitoramento WHERE samid = '.$_REQUEST['samid']); }
                    $val = (!empty($samdesc)) ? stripslashes($samdesc) : '';
                    echo campo_texto('samdesc', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="samdesc"', '', $val);
                ?>
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
        </tr>

        <tr bgcolor="#C0C0C0" style="text-align: center">
            <td colspan="3">
                <input type='button' class='botao' name='Salvar'   value='Salvar'   id='SalvarS'   onclick="validar('formulario_subtipo')"/>
                <input type='button' class='botao' name='Cancelar' value='Cancelar' id='cancelarS' onclick="window.reload()"/>
            </td>
        </tr>
    </table>
</form>
    
<?php $objMonitoramento->montaListaSubtipoAtividadesMonitoramento(); ?>

<script type="text/javascript">

    function validar(idForm) {
        var mensagem  = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
        var validacao = true;

        if(idForm == 'formulario_tipo'){
            var tamdesc     = $('#tamdesc').val();
            if (tamdesc == '') {
                mensagem += 'O campo de Nome do Tipo deve ser preenchido.\n';
                validacao = false;
                $('#tamdesc').focus();
            }
            
            if (!validacao) {
                alert(mensagem);
                return false;
            } else {
                if($('#tamid').val() == ''){
                    $('#requisicaoT').val('salvar');
                }else{
                    $('#requisicaoT').val('editar');
                }
                $('#formulario_tipo').submit();
            }
        }

        if(idForm == 'formulario_subtipo'){
            var samdesc     = $('#samdesc').val();
            if (samdesc == '') {
                mensagem += 'O campo de Nome do Subtipo deve ser preenchido.\n';
                validacao = false;
                $('#samdesc').focus();
            }
            
            if (!validacao) {
                alert(mensagem);
                return false;
            } else {
                if($('#samid').val() == ''){
                    $('#requisicaoS').val('salvar');
                }else{
                    $('#requisicaoS').val('editar');
                }
                $('#formulario_subtipo').submit();
            }
        }
    }
</script>

</body>
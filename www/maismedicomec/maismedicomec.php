<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>


<?php 
    $pagData = explode('/' , $_GET['modulo']);
    $pag = reset($pagData);
    
    if($pag != 'sistema'): 
        $perfil = (array) pegaPerfilGeral($_SESSION['usucpf']);
?>
    <?php if( in_array(PERFIL_CONSULTA, $perfil) ): ?>
        <script language="JavaScript">
            $('input[type=text] , input[type=radio] , input[type=checkbox] , textarea , input[value=Salvar] , #formulario select').attr('disabled' , 'disabled');
            $('img[src="/imagens/excluir.gif"]').attr('src' , '../imagens/excluir_01.gif').removeAttr('onclick').removeAttr('style');
        </script>
    <?php endif; ?>
<?php endif; ?>
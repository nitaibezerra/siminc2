<?PHP
    if($_REQUEST['requisicao']=='pesquisarCurso'){
        header('Content-Type: text/html; charset=iso-8859-1');
        pesquisarCurso(null,$_REQUEST);
        exit();
    }

    if($_REQUEST['requisicao']=='excluirCurso'){
        header('Content-Type: text/html; charset=iso-8859-1');
        excluirCurso($_REQUEST);
        pesquisarCurso(null, $_REQUEST);
        exit();
    }

    if($_REQUEST['requisicao']=='gerarExcel'){
        header('Content-Type: text/html; charset=iso-8859-1');
        gerarExcel($_REQUEST);
        exit();
    }

    if( $_REQUEST['requisicao'] == 'exibirListaAtividades') {
        exibirListaAtividades($_REQUEST);
    }

    if(!$_SESSION['sisfor']['unicod'] || ($_REQUEST['unicod'] && $_REQUEST['unicod'] != $_SESSION['sisfor']['unicod'])){
        $_SESSION['sisfor']['unicod'] = $_REQUEST['unicod'];
    }

    if(!$_SESSION['sisfor']['curid'] || ($_REQUEST['curid'] && $_REQUEST['curid'] != $_SESSION['sisfor']['curid'])){
        $_SESSION['sisfor']['curid'] = $_REQUEST['curid'];
    }

    require_once APPRAIZ . "includes/cabecalho.inc";

    $perfis = recuperaPerfil();

    $comp = new AssInt();
    $co_ies_filtro = $comp->pegaIES();

    if( in_array(PERFIL_PEC_G, $perfis ) ){
        $sub = exibirNomeIES($co_ies_filtro[0]);
    } else {
        $sub = "";
    }

    #MONTA TÍTULO DA PÁGINA
    echo "<br>";
    $db->cria_aba($abacod_tela,$url,'');
    monta_titulo('PEC-G',$sub);

    $ano = $_GET['ano'];
    $aba = !$_GET['aba'] ? "listaPEC_G" : $_GET['aba'];

    if( $ano != '' ){
        $abaAtiva = "assint.php?modulo=principal/PEC_G&acao=A&aba={$aba}&ano={$ano}";
    }else{
        $abaAtiva = "assint.php?modulo=principal/PEC_G&acao=A&aba={$aba}";
    }

    #ARRAY QUE CONSTROE AS ABA DA PEC-G E VAGAS 2015 DOS SEUS RESPECTIVOS CURSOS - VISUALIZÃO POR TODOS USUÁRIOS.
    $menu = array(
        0 => array(
            "id" => 1,
            "descricao" => "Lista PEC-G",
            "link" => "assint.php?modulo=principal/PEC_G&acao=A&aba=listaPEC_G"
        )
    );

    #ABA ATIVIDADES REALIZADAS PELO USUÁRIO EM PEC G - VISUALIZÃO SUPER USUÁRIO E ADMIM
    if( in_array(PERFIL_SUPER_USUARIO, $perfis ) || in_array(PERFIL_ADMINISTRADOR, $perfis ) ){
        array_push( $menu, array("id" => 1, "descricao" => "Atividades Realizadas PEC-G", "link" => "assint.php?modulo=principal/PEC_G&acao=A&aba=listagem_ativid_pec") );
    }

    #ABA DAS VAGAS 2015 DOS SEUS RESPECTIVOS CURSOS - VISUALIZÃO POR TODOS USUÁRIOS.
    array_push( $menu, array("id" => 2, "descricao" => "Vagas 2015", "link" => "assint.php?modulo=principal/PEC_G&acao=A&aba=vagasPEC_G&ano=2015") );

    #ABA ATIVIDADES REALIZADAS PELO USUÁRIO EM PEC G - VISUALIZÃO SUPER USUÁRIO E ADMIM
    if( in_array(PERFIL_SUPER_USUARIO, $perfis ) || in_array(PERFIL_ADMINISTRADOR, $perfis ) ){
        array_push( $menu, array("id" => 3, "descricao" => "Atividades Realizadas Vagas 2015", "link" => "assint.php?modulo=principal/PEC_G&acao=A&aba=listagem_ativid_vagas") );
    }

    #ABA DAS VAGAS 2015 DOS SEUS RESPECTIVOS CURSOS - VISUALIZÃO POR TODOS USUÁRIOS.
    array_push( $menu, array("id" => 4, "descricao" => "Vagas 2016", "link" => "assint.php?modulo=principal/PEC_G&acao=A&aba=vagasPEC_G&ano=2016") );
    
    #ABA ATIVIDADES REALIZADAS PELO USUÁRIO EM PEC G - VISUALIZÃO SUPER USUÁRIO E ADMIM
    if( in_array(PERFIL_SUPER_USUARIO, $perfis ) || in_array(PERFIL_ADMINISTRADOR, $perfis ) ){
        array_push( $menu, array("id" => 3, "descricao" => "Atividades Realizadas Vagas 2016", "link" => "assint.php?modulo=principal/PEC_G&acao=A&aba=listagem_ativid_vagas&ano=2016") );
    }

    echo "<br/>";
    echo montarAbasArray($menu, $abaAtiva);
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script language="javascript" type="text/javascript">
    jQuery.noConflict();

    jQuery('.adicionar').live('click',function(){
        var ano = $('#ano').val();
        return window.open('assint.php?modulo=principal/cadCursoPEC_G&acao=A&ano='+ano,'modelo',"height=600,width=850,scrollbars=yes,top=50,left=200");
    });

    function alterarCurso(ofpid){
        var ano = $('#ano').val();
        return window.open('assint.php?modulo=principal/cadCursoPEC_G&acao=A&ano='+ano+'&ofpid='+ofpid,'modelo',"height=650,width=850,scrollbars=yes,top=50,left=200");
    }

    function excluirCurso(ofpid){
        var ano = $('#ano').val();
        
        if(confirm("Deseja realmente excluir o Curso?")){
            divCarregando();
            jQuery.ajax({
                url: 'assint.php?modulo=principal/PEC_G&acao=A&aba=vagasPEC_G',
                data: { requisicao: 'excluirCurso', ofpid: ofpid, ano: ano}, 
                async: false,
                success: function(data) {
                    if(trim(data)){
                        alert('Curso excluído com sucesso!');
                        jQuery("#divCurso").html(data);
                    } else {
                        alert('Não foi possível excluir o curso!');
                        jQuery("#divCurso").html(data);
                    }
                }
            });
            divCarregado();
        }
    }

    function pesquisarCurso(){

        selectAllOptions(document.getElementById('co_ies'));

        jQuery('#requisicao').val('pesquisarCurso');

        divCarregando();

        jQuery.ajax({
            type: 'POST',
            url: 'assint.php?modulo=principal/PEC_G&acao=A&aba=vagasPEC_G',
            data: jQuery('#formulario').serialize(),
            async: false,
            success: function(data) {
                jQuery("#divCurso").html(data);
                divCarregado();
            }
        });
    }


    function gerarExcel(){
        jQuery('#requisicao').val('gerarExcel');
        jQuery('#formulario').submit();
    }
</script>

<?PHP

    include_once $aba.".inc";

?>
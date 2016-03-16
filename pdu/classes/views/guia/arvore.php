<?PHP




/*
class Simec_tree{



}

#icone que abre a árvore.
function tree_acaoes_Plus(){
    echo "
        <span>
            <i class=\"glyphicon glyphicon-plus-sign\"></i>
        </span>
    ";
}
#icone que fecha a árvore.
function tree_acaoes_Minus(){
    echo "
        <span>
            <i class=\"glyphicon glyphicon-minus-sign\"></i>
        </span>
    ";
}

function tree_acaoes_Add(){
    echo "
        <button class=\"badge badge-success\">
            <i class=\"glyphicon glyphicon-plus\"></i>
        </button>
    ";
}

function tree_acaoes_edit(){
    echo "
        <button type=\"button\" class=\"badge badge-warning\">
            <i class=\"glyphicon glyphicon-pencil\"></i>
        </button>
    ";

}

function tree_acaoes_del(){
    echo "
        <button type=\"button\" class=\"badge badge-important\">
            <i class=\"glyphicon glyphicon-trash\"></i>
        </button>
    ";
}

function tree_acaoes_serch(){
    echo "
        <button type=\"button\" class=\"badge badge-info\" onclick=\"abrir('{$dados['codigo']}')\">
            <i class=\"glyphicon glyphicon-search\"></i>
        </button>
    ";
}


function busca_result_sql( $SQL ){
    global $db;

    //ver($SQL, d);

    #MONTA A SQL PAI
    return $db->carregar( $SQL );
}


#funcao simec_tree
function simec_tree_v2( $config_tree ){
    global $db;

    $qtd_nivel = $config_tree['qtd_nivel'];

    echo "<div class=\"col-lg-12\">";
        echo "<div class=\"tree\">";

            #ABRE AS AÇÕES.
            foreach( $config_tree['config'] as $conf_nivel){ ver($conf_nivel);
                escrever_nivel( $conf_nivel );
            }

            #FECHA AS AÇÕES.
            foreach( $config_tree['config'] as $conf_nivel){
                    echo "</li>";
                echo "</ul>";
            }

        echo "</div>";
    echo "</div>";
}

#funcao simec_tree
function escrever_nivel( $arry_acoes ){

    #ESCREVER OS DADOS
    $result = busca_result_sql( $arry_acoes['sql'] );

    foreach( $result as $key => $dados ){
        echo "<ul>";
            echo "<li>";

            foreach( $arry_acoes['acoes'] as $acoes ){
                switch ($acoes) {
                    case 'min':
                        tree_acaoes_Minus();
                        break;
                    case 'plu':
                        tree_acaoes_Plus();
                        break;
                    case 'sch':
                        tree_acaoes_serch();
                        break;
                    case 'add':
                        tree_acaoes_Add();
                        break;
                    case 'edt':
                        tree_acaoes_edit();
                        break;
                    case 'del':
                        tree_acaoes_del();
                        break;
                }
            }

            #DESCRIÇÃO
            echo $dados["descricao"];

        if( $key == 0 ){
                echo "</li>";
            echo "</ul>";
        }
    }
}


$sql_pai = "SELECT  itrid AS codigo, itrdsc AS descricao FROM pdu.instrumento";
$sql_filho_0 = "SELECT  dimid AS codigo, dimdsc AS descricao FROM pdu.dimensao WHERE dimstatus = 'A'";
$sql_filho_1 = "SELECT  areid AS codigo, aredsc AS descricao FROM pdu.area WHERE arestatus = 'A'";
$sql_filho_2 = "SELECT  indid AS codigo, inddsc AS descricao FROM pdu.indicador WHERE indstatus = 'A'";

$config_tree = array(
    'config' => array(
        'pai_0' => array(
            'nivel' => 0,
            'acoes' => array('min','add','sch'),
            'sql'   => 'SELECT  itrid AS codigo, itrdsc AS descricao FROM pdu.instrumento'
        ),
        'filho_1' => array(
            'nivel' => 1,
            'acoes' => array('min','add','edt','del'),
            'c_pai' => 'itrid',
            'sql'   => 'SELECT  dimid AS codigo, dimdsc AS descricao FROM pdu.dimensao WHERE dimstatus = \'A\''
        ),
//        'filho_2' => array(
//            'nivel' => 2,
//            'acoes' => array('min','add','edt','del'),
//            'sql'   => 'SELECT  areid AS codigo, aredsc AS descricao FROM pdu.area WHERE arestatus = \'A\''
//        ),
//        'filho_3' => array(
//            'nivel' => 3,
//            'acoes' => array('min','add','edt','del'),
//            'sql'   => 'SELECT  indid AS codigo, inddsc AS descricao FROM pdu.indicador WHERE indstatus = \'A\''
//        )
    )

);


//simec_tree_v2( $config_tree );


#funcao simec_tree
function simec_tree( $array_tree_pai, $array_tree_filho ){
    global $db;
//    ver( $array_tree, d);

    echo "<div class=\"col-lg-12\">";
        echo "<div class=\"tree\">";
            #INICIO DA LISTA PAI - <ul>
            echo "<ul>";
                #PAI
                if( $array_tree_pai['sql_pai'] != '' ){
                    $_pai = busca_sql_pai( $array_tree_pai['sql_pai'] );

                    if( $_pai != '' ){
                        foreach( $_pai as $dados ){
                            #INICIO DA LINHA - <li>
                            echo "<li>";
                                #AÇÕES
                                tree_acaoes_Minus();
                                tree_acaoes_Add();
                                tree_acaoes_serch();

                                #DESCRIÇÃO
                                echo $dados["descricao"];

                                #INICIO DA LISTA FILHO - <ul>
                                echo "<ul>";

                                #FILHO
                               $controle = 0;
                               foreach( $array_tree_filho as $filho ){ //ver($array_tree_filho, $filho, d);

                                    #FILHO
                                    $_filho = busca_sql_filho( $filho );

                                    foreach( $_filho as $dados ){
                                        echo "<li>";

                                            tree_acaoes_Minus();
                                            tree_acaoes_Add();
                                            tree_acaoes_edit();
                                            tree_acaoes_del();

                                            echo $dados['descricao'];

                                            #INICIO DA LISTA FILHO DO FILHO - <ul>
                                            if( $controle == 1 && $_filho != '' ){ //ver($filho, d);
                                                echo "<ul>";

                                                    #FILHO DO FILHO
                                                    $_filho_filho = busca_sql_filho( $filho );

                                                    foreach( $_filho_filho as $dados ){
                                                        echo "<li>";

                                                            tree_acaoes_Minus();
                                                            tree_acaoes_Add();
                                                            tree_acaoes_edit();
                                                            tree_acaoes_del();

                                                            echo $dados['descricao'];

                                                            echo "<ul>";

                                                            echo "</ul>";

                                                        echo "</li>";

                                                    }//fim do foreach (filho do filho)


                                                echo "</ul>";
                                            }
                                        echo "</li>";
                                    }//fim do foreach (filho) - dimensao
                                    $controle = $controle + 1;
                                }
                            echo "</ul>";
                        echo "</li>";
                        }//fim do foreach (pai) - instrumento
                    }//fim do if (pai)
                }//if ( pai ) verifica se sera criada a arvore pai
            echo "</ul>";
        echo "</div>";
    echo "</div>";

}
*/

?>

<link type="text/css" rel="stylesheet" media="screen" href="../pdu/css/tree_bootstrap_combined.css">

<script type="text/javascript">

    $(function () {
        $('.tree li:has(ul)').addClass('parent_li').find(' > span').attr('title', 'Abrir Árvore');

        $('.tree li.parent_li > span').on('click', function (e) {
            var children = $(this).parent('li.parent_li').find(' > ul > li');

            if (children.is(":visible")){
                children.hide('fast');
                $(this).attr('title', 'Expandir a árovore').find(' > i').addClass('glyphicon-plus-sign').removeClass('glyphicon-minus-sign');
            } else {
                children.show('fast');
                $(this).attr('title', 'Fechar a árvore').find(' > i').addClass('glyphicon-minus-sign').removeClass('glyphicon-plus-sign');
            }
            e.stopPropagation();
        });
        //CHAMA A FUNÇÃO E COMO A ÁRVORE ESTA ABERTA!
        $('#tree').find('li.parent_li span').click();
    });

    function fecharTodos(){
        $('.tree li.parent_li > span').parent('li.parent_li').find(' > ul > li').hide('fast');
        $('.tree li.parent_li > span').attr('title', 'Expandir a árovore').find(' > i').addClass('glyphicon-plus-sign').removeClass('glyphicon-minus-sign');
    }

    function abrirTodos(){
        $('.tree li.parent_li > span').parent('li.parent_li').find(' > ul > li').show('fast');
        $('.tree li.parent_li > span').attr('title', 'Expandir a árovore').find(' > i').addClass('glyphicon-minus-sign').removeClass('glyphicon-plus-sign');
    }

    function fecharModal(){
        $('#modal').modal('hide');
        $.post(window.location.href, {controller: 'guia', action: 'arvore'}, function(html) {
            $('#container_arvore').hide().fadeIn().html(html);
        });
    }

    // ------------------------------------------------ DELETAR -------------------------------------------------- //
    function deletar_dimensao(id , element){
        $.deleteItem({controller: 'guia', action: 'excluirDimensao', text : 'Deseja realmente deletar excluir essa Dimensão?', id: id, functionSucsess: 'carregarArvore' });
    }

    function deletar_area(id , element){
        $.deleteItem({controller: 'guia', action: 'excluirArea', text : 'Deseja realmente deletar excluir essa Área?', id: id, functionSucsess: 'carregarArvore' });
    }

    function deletar_indicador(id , element){
        $.deleteItem({controller: 'guia', action: 'excluirIndicador', text : 'Deseja realmente deletar excluir esse Indicador?', id: id, functionSucsess: 'carregarArvore' });
    }

    function deletar_criterio(id , element){
        $.deleteItem({controller: 'guia', action: 'excluirCriterio', text : 'Deseja realmente deletar excluir esse Critério?', id: id, functionSucsess: 'carregarArvore' });
    }

    // -------------------------------------------------- EDITAR ---------------------------------------------------- //
    function edit_dimensao(itrid, dimid, element){
        $.post(window.location.href, {controller: 'guia', action: 'formularioDimensao', itrid: itrid, dimid: dimid, tipo_acao: 'up'}, function(html) {
            $('#modal').html(html).modal('show');
        });
    }

    function edit_area(dimid, areid, element){
        $.post(window.location.href, {controller: 'guia', action: 'formularioArea', dimid: dimid, areid: areid, tipo_acao: 'up'}, function(html) {
            $('#modal').html(html).modal('show');
        });
    }

    function edit_indicador(areid, indid, element){
        $.post(window.location.href, {controller: 'guia', action: 'formularioIndicador', areid: areid, indid: indid, tipo_acao: 'up'}, function(html) {
            $('#modal').html(html).modal('show');
        });
    }

    function edit_criterio(indid, crtid, element){
        $.post(window.location.href, {controller: 'guia', action: 'formularioCriterio', indid: indid, crtid: crtid, tipo_acao: 'up'}, function(html) {
            $('#modal').html(html).modal('show');
        });
    }

    // ----------------------------------------------- ABRIR FORMULARIO --------------------------------------------- //
    function formulario_dimensao(id , element){
        $.post(window.location.href , { controller: 'guia' , action: 'formularioDimensao' , itrid: id, tipo_acao: 'in'} , function(html){
            $('#modal').html(html).modal('show');
        });
    }

    function formulario_area(id , element){
        $.post(window.location.href , { controller: 'guia' , action: 'formularioArea' , dimid: id, tipo_acao: 'in'} , function(html){
            $('#modal').html(html).modal('show');
        });
    }

    function formulario_indicador(id , element){
        $.post(window.location.href , { controller: 'guia' , action: 'formularioIndicador' , areid: id, tipo_acao: 'in'} , function(html){
            $('#modal').html(html).modal('show');
        });
    }

    function formulario_criterio(id , element){
        $.post(window.location.href , { controller: 'guia' , action: 'formularioCriterio' , indid: id, tipo_acao: 'in'} , function(html){
            $('#modal').html(html).modal('show');
        });
    }

    function limparFormulario(){
        $('#form_save').find('input[type!=hidden], textarea').not(':disabled').val('');
    }

    function carregarArvore(){

        limparFormulario();

        $.post(window.location.href , {controller: 'guia' , action : 'arvore'} , function(html){
            $('#container_arvore').hide().html(html).fadeIn();
        });
    }
</script>

<br>

<div class="col-lg-12">
    <div class="panel panel-primary">
        <div class="panel-heading col-1" style="text-align: center;">
            <span class="">
                <!--<i class="glyphicon glyphicon-sort-by-attributes"></i>-->
                Guia de Ações Padronizadas
            </span>
        </div>
    </div>
</div>

<?PHP

    arvore_pdu();

?>
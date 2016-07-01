<?php include_once APPRAIZ . "includes/library/simec/Grafico.php"; ?>
<?php include_once '../planacomorc/_funcoesOrcamentario.php'; ?>
<?php include_once APPRAIZ . "includes/painel_slide_worflow.php"; ?>
<?php include_once APPRAIZ . "includes/painel_slide_automatico.php"; ?>

<?php
if($_POST['requisicaoAjaxPainel']){
    $_POST['requisicaoAjaxPainel']();
    exit;
}
?>
    <link href="../library/sliding_panel/css/slide.css" rel="stylesheet" media="screen">
    <script src="../library/jquery/jquery-1.11.1.min.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>
<!--    <script src="../library/bootstrap-switch-master-3.0/js/bootstrap-switch.min.js"></script>-->
    <script src="../library/chosen-1.0.0/chosen.jquery.js"></script>
    <style type="text/css">
        .div_conteudo{
            /*background-color: #f5f5f5;*/
            background-color: #FFFFFF;
            /*border: 1px solid #e3e3e3;*/
            border-radius: 4px;
            /*box-shadow: 0 1px 1px rgba(0, 0, 0, 0.05) inset;*/
            font-size: xx-small;
            margin-bottom: 10px;
            min-height: 20px;
            /*padding: 10px;*/
            padding: 5px 10px 5px 10px;
            /*text-decoration: none;*/
            /*width: 98%;*/

            /*min-height: 20px;*/
            /*padding: 19px;*/
            /*margin-bottom: 20px;*/
            /*background-color: #EEEEEE;*/
            border: 1px solid #C2C2C2;
            /*border-radius: 4px;*/
            /* -webkit-box-shadow: inset 0 1px 1px rgba(0, 0, 0, 0.05); */
            /* box-shadow: inset 0px 0px 6px 0px rgba(0, 0, 0, 0.18); */
            /* box-shadow: inset 0px 0px 6px 0px rgba(0, 0, 0, 0.24); */
            box-shadow: inset 1px 1px 6px 0px rgba(0, 0, 0, 0.24)
        }

        .div_conteudo h1{
            color: #15ADFF !important;
        }

        .div_conteudo h1.tituloPainel{
            border-bottom: 1px solid #e5e5e5;
        }
    </style>

    <script language="javascript">
        $1_11 = $.noConflict();
    </script>

    <!-- Panel -->
    <div id="toppanel" class="notprint">
        <div id="panel_geral">
            <div id="panel_content">
                <div id="conteudo_painel" style="padding: 15px;">
                    <?php
                    if ($db->testa_superuser()) {
                        $sql = "select count(*) as qtd
                                from painel.acaosistema asi
                                where asi.sisid = {$_SESSION['sisid']}";
                        $qtdFinanceiros = $db->pegaUm($sql);

                        $sql = "select count(*) as qtd from public.grafico where sisid = {$_SESSION['sisid']}";
                        $qtdGrafico = $db->pegaUm($sql);

                        $active = null;
                        foreach($painelCabecalho as $painel) {
                            if (in_array('active', $painel)) {
                                $active = 1;
                                break;
                            }
                        }

                        if ($qtdFinanceiros) {
                            $activeFin = $qtdGrafico || $active ? null : 'active';
                            $painelCabecalho[] = array('titulo' => 'Financeiro', 'funcao' => 'montarPainelFinanceiro', 'icon' => 'signal', $activeFin);
                        }

                        if ($qtdGrafico) {
//                            $active = $active ? null : 'active';
                            $painelCabecalho[] = array('titulo' => 'Indicadores', 'funcao' => 'montarPainelIndicadores', 'icon' => 'flag', $active);
                        }
                    }

                    if (isset($painelCabecalho) && is_array($painelCabecalho)) : ?>
                        <?php foreach($painelCabecalho as $painel): ?>
                            <div class="div_conteudo">
                                <div class="head">
                                    <a href="javascript:void(0);" onclick="abrirConteudo(this, '<?php echo $painel['funcao'] ?>');" class="<?php echo in_array('active', $painel) ? 'painel_active' : '' ?>">
                                        <h1>
                                            <span class="glyphicon glyphicon-<?php echo isset($painel['icon'])  ? $painel['icon'] : ''; ?>" ></span>
                                            &nbsp; <?php echo $painel['titulo'] ?>
                                            <i class="glyphicon glyphicon-arrow-down" style="float: right;" ></i>
                                        </h1>
                                    </a>
                                </div>
                                <div class="body"></div>
                            </div>
                        <?php endforeach; ?>
                    <?php endif;

                    ?>
                </div>
            </div>
        </div> <!-- /login -->

        <!-- The tab on top -->
        <div class="tab">
            <ul class="login">
                <li class="left">&nbsp;</li>
                <li id="toggle">
                    <a id="open" href="#">Painel</a>
                    <a id="close" style="display: none;" href="#">Fechar</a>
                </li>
                <li class="right">&nbsp;</li>
            </ul>
        </div> <!-- / top -->
    </div> <!--panel -->

    <script type="text/javascript">

        jQuery(document).ready(function() {

<!--            $1_11('#conteudo_painel').load('--><?php //echo $painelCabecalho; ?><!--');-->

            jQuery('#content_top_simula').css('position' , 'absolute');
            jQuery('#content_top_simula').css('z-index' , '1000');
            jQuery('#top_content_logo_menu').css('position' , 'absolute');
            jQuery('#top_content_logo_menu').css('z-index' , '1000');

            // Expand Panel
            jQuery("#open").click(function(){
                jQuery("div#panel_geral").slideDown("slow");
                jQuery('#content_top_simula').css('z-index' , '1');
                jQuery('#top_content_logo_menu').css('z-index' , '1');
            });

            // Collapse Panel
            jQuery("#close").click(function(){
                jQuery("div#panel_geral").slideUp("slow");
                setTimeout(function(){
                    jQuery('#content_top_simula').css('z-index' , '1000');
                    jQuery('#top_content_logo_menu').css('z-index' , '1000');
                },500);
            });

            // Switch buttons from "Log In | Register" to "Close Panel" on click
            jQuery("#toggle a").click(function () {
                jQuery("#toggle a").toggle();
            });

            jQuery('a.painel_active').click();
        });
    </script>

    <script language="javascript">
        function abrirConteudo(elemento , metodo){
            var elemento = $1_11(elemento);
            var icone = elemento.find('i');
            var h1 = elemento.find('h1');
            var corpo = elemento.closest('.head').next('.body');

            var valorDefault = '<h1 style="text-align: center; color: #000000;"><img src="../imagens/carregando.gif"> Carregando...</h1>';
            corpo.html(valorDefault);
            h1.addClass('tituloPainel');

            icone.attr('class','glyphicon glyphicon-arrow-up');
            elemento.attr('onclick','javascript:fecharConteudo(this, "' + metodo + '");');
            console.info(window.location.href);

            if($1_11.trim(corpo.html()) == valorDefault){
                $1_11.post(window.location.href, {requisicaoAjaxPainel: metodo} , function(html){
                    corpo.hide().html(html).fadeIn();
                });
                console.info('Carregou ajax.');
            } else {
                corpo.fadeIn();
                console.info('Não carregou ajax de novo.');
            }
        }

        function fecharConteudo(elemento , metodo){
            var elemento = $1_11(elemento);
            var icone = elemento.find('i');
            var h1 = elemento.find('h1');
            var corpo = elemento.closest('.head').next('.body');

            h1.removeClass('tituloPainel');

            icone.attr('class','glyphicon glyphicon-arrow-down');
            elemento.attr('onclick','javascript:abrirConteudo(this, "' + metodo + '");');

            corpo.fadeOut();
        }
    </script>
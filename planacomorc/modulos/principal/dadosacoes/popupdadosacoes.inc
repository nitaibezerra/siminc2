<?php
    require_once APPRAIZ . "includes/funcoesspo_componentes.php";
?>
<html>
    <head>
    </head>
    <body>    
        <div class="col-lg-12">
            <div class="well">
                <fieldset>
                    <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
                        <div class="form-group">
                            <title>Aviso!</title>
                        </div>
                        <div>                            
                            <div for="inputUnidade" class="col-md-10" style="text-align:justify">
                                Prezado(a), <br/> <br/> 
                                Por favor descreva abaixo da forma mais detalhada possível qual valor está incorreto e, 
                                se possível, informe o valor correto para que possamos identificar a fonte do erro e efetuar a correção. <br/><br/> 
                                Deste já agradecemos. <br/><br/> 
                                Equipe SIMEC.<br><br>
                            </div>
                        </div>
                        <div>
                            <section class="col-md-10">
                                <div>
                                    <?php inputTextArea('justificativa', $valor, 'justificativa', '300',array('cols'=>60,'class'=>'normal form-control'));?>
                                </div>
                            </section>
                        </div>
                        <div id="requisicao"></div>
                        <div id="origem"><input type="hidden" name="origem" value="<?php echo $_REQUEST['tipo'];?>"></div>
                    </form>
                </fieldset>
            </div>
        </div>
    </body>
</html>
<?php die();
<?php 
global $db;
//situação de reload da tela, exceto pela função limpar campos. Populando campos e recarregando lista da sessão.
if(isset($_SESSION['dados_filtro_documento']) && !empty($_SESSION['dados_filtro_documento'])){
        $_POST = $_SESSION['dados_filtro_documento'];
        //setando valores nos campos para manter pesquisa anterior
        $chkreiteracao = ( isset($_POST['chkreiteracao']) && ($_POST['chkreiteracao'] === 'on') ) ? 'checked = "checked"' : ''; 
        $chkAtrasado = (isset($_POST['chkAtrasado']) && ($_POST['chkAtrasado'] === 'on') ) ? 'checked = "checked"' : '';
        $this->entity['dmdassunto']['value'] = !empty($_SESSION['dados_filtro_documento']['dmdassunto']) ? $_SESSION['dados_filtro_documento']['dmdassunto'] : '';
        //manter demais campos carregados
        echo "<script type='text/javascript'>
                $(function(){
                    $( window ).load(function() {        
                        manterLista();
                    });
                });
              </script>";
}
?>
<div class="container">

    <?php echo montarAbasArray($_SESSION['demandasse']['abas_array'], $_SESSION['demandasse']['url'], ''); ?>
    <form id="form_search" method="post" class="form-horizontal">
        <input name="controller" id="controller" value="documento" type="hidden"/>
        <input name="action" id="action" value="" type="hidden"/>
        <input name="pesquisar" id="pesquisar" value="" type="hidden"/>
        <input name="apagarPesquisaSessao" id="apagarPesquisaSessao" value="" type="hidden" />

        <div class="col-md-12">
            <div class="well">
                <fieldset>
                    <legend>Pesquisa</legend>
                    <div class="col-md-1"></div>
                    <div class="col-md-10">
                        <div class="form-group">
                            <label for="tpdid" class="col-lg-2 col-md-2  control-label">Tipo</label>
                            <div class="col-lg-10 col-md-10 ">
                                <div class="btn-group" data-toggle="buttons">
                                    <?php foreach ($this->tipoDocumento as $tipoDocumento): ?>
                                        <label
                                            class="btn btn-default <?php if ($this->entity['tpdid']['value'] == $tipoDocumento['tpdid']) echo 'active' ?>">
                                            <input id="tpdid" name="tpdid" type="radio"
                                                   value="<?php echo $tipoDocumento['tpdid'] ?>" <?php if ($this->entity['tpdid']['value'] == $tipoDocumento['tpdid']) echo 'checked="checked"' ?>>
                                            <?php echo $tipoDocumento['tpddsc'] ?>
                                        </label>
                                    <?php endforeach; ?>
                                </div>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="dmdid" class="col-lg-2 col-md-2 control-label">Número</label>

                            <div class="col-lg-2 col-md-2 ">
                                <input id="dmdnumdocumento" name="dmdnumdocumento" type="text" class="form-control" placeholder="" value="">
                            </div>
                        </div>
                            <div class="form-group">
                                <label class="col-lg-2 col-md-2 control-label">Situação</label>
                                <div class="col-lg-5 col-md-5  ">
                                    <select id="esdid" name="esdid" class="form-control" data-placeholder="Selecione">
                                            <option value="0">Todos</option>
                                            <?php 								
                                                $sqlDoc = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = ".WF_TPDID_DEMANDASSE_DEMANDA." order by esdordem";
                                                $resDoc = $db->carregar($sqlDoc);
                                                foreach($resDoc as $doc){?>
                                                    <option value="<?php echo $doc['codigo'] ?>"><?php echo $doc['descricao'] ?></option><?php
                                            }
                                            ?>
                                    </select>
							</div>
						</div>
                        <div class="form-group">
                            <label for="dmddtentdocumento_search" class="col-lg-2 col-md-2  control-label">Data do documento</label>
                            <div class="col-lg-2 col-md-2  ">
                                <input id="dmddtentdocumento_search" name="dmddtentdocumento" type="text" class="form-control" placeholder="dd/mm/aaaa" value="">
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="dmddtemidocumento_search" class="col-lg-2 col-md-2  control-label">Data de publicação</label>
                            <div class="col-lg-2 col-md-2  ">
                                <input id="dmddtemidocumento_search" name="dmddtemidocumento" type="text" class="form-control" placeholder="dd/mm/aaaa" value="">
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="dmdassunto" class="col-lg-2 col-md-2  control-label">Assunto</label>
                            <div class="col-lg-5 col-md-5  ">
                                <?php 
                                    echo inputTextArea('dmdassunto', $this->entity['dmdassunto']['value'], 'dmdassunto', 500, array( 'cols'=>60,'rows'=>4));
                                ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="prcid_orig" class="col-lg-2 col-md-2  control-label">Interessado</label>

                            <div class="col-lg-5 col-md-5  ">
                                <select id="prcid_orig" name="prcid_orig" class="form-control"
                                        data-placeholder="Selecione">
                                    <option></option>
                                    <?php foreach ($this->procedencias as $procedencia): ?>
                                        <option
                                            value="<?php echo $procedencia['prcid'] ?>"><?php echo $procedencia['prcdsc'] ?></option>
                                    <?php endforeach; ?>
                                </select>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="prcid_dest" class="col-lg-2 col-md-2  control-label">Destino</label>

                            <div class="col-lg-5 col-md-5  ">
                                <select id="prcid_dest" name="prcid_dest" class="form-control"
                                        data-placeholder="Selecione">
                                    <option></option>
                                    <?php foreach ($this->procedencias as $procedencia): ?>
                                        <option
                                            value="<?php echo $procedencia['prcid'] ?>"><?php echo $procedencia['prcdsc'] ?></option>
                                    <?php endforeach; ?>
                                </select>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="dmdreferencia" class="col-lg-2 col-md-2  control-label">Referência</label>

                            <div class="col-lg-5 col-md-5  ">
                                <input id="dmdreferencia" name="dmdreferencia" type="text" class="form-control"
                                       placeholder="" value="">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-lg-2 col-md-2 control-label">Reiteração</label>
                            
                            <div class="col-lg-2 col-md-2">
                                <input type="checkbox" name="chkreiteracao" id="chkreiteracao" <?php echo $chkreiteracao;?> />
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="dmdnumsidoc" class="col-lg-2 col-md-2  control-label">Sidoc</label>

                            <div class="col-lg-5 col-md-5  ">
                                <input id="dmdnumsidoc" name="dmdnumsidoc" type="text" class="form-control"
                                       placeholder="" value="">
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="dmdprazoemdata_1_search" class="col-lg-2 col-md-2  control-label">Prazo em data</label>

                            <div class="col-lg-2 col-md-2  ">
                                <input id="dmdprazoemdata_1_search" name="dmdprazoemdata_begin" type="text" class="form-control"
                                       placeholder="dd/mm/aaaa" value="">
                            </div>
                            <div class="col-lg-1 col-md-1 text-center "
                                 style="min-height: 34px; text-align: center; padding:10px 0px 0px 0px; width: 10px">à
                            </div>
                            <div class="col-lg-2 col-md-2  ">
                                <input id="dmdprazoemdata_2_search" name="dmdprazoemdata_end" type="text" class="form-control"
                                       placeholder="dd/mm/aaaa" value="">
                            </div>
                        </div>
                        <div class="form-group">
                        	<label class="col-lg-2 col-md-2 control-label">Em Atraso</label>
                        	
                        	<div class="col-lg-2 col-md-2">
                        		<input type="checkbox" name="chkAtrasado" id="chkAtrasado" <?php echo $chkAtrasado;?> />
                        	</div>
                        </div>
                          <div class="form-group">
                            <label for="prcid_orig" class="col-lg-2 col-md-2  control-label">Ordenar por:</label>

                            <div class="col-lg-5 col-md-5  ">
                                <select class="form-control" id="ordenacao" name="ordenacao"   data-placeholder="Selecione">
                                    <option></option>
                                    <option value='dmdnumdocumento'>Nº Documento</option>
                                    <option value='dmdprazoemdata'>Prazo em Data</option>
                                    
                                </select>
                              
                            </div>
                            
                        </div>
                                 <div class="form-group">
                            <label for="prcid_orig" class="col-lg-2 col-md-2  control-label">Tipo de Ordenação:</label>

                            <div class="col-lg-5 col-md-5  ">
                                 <div class="btn-group" data-toggle="buttons">
                                  <label class="btn btn-default">
                                <input type="radio" name="tipoOrdenacao"
                                       id="tipoOrdenacao"
                                       value="DESC" />Decrescente
                            </label>
                            <label class="btn btn-default">
                                <input type="radio" name="tipoOrdenacao"
                                       id="tipoOrdenacao"
                                       value="ASC" />Ascendente
                            </label>
<!--                                <input type="radio" name="tipoOrdenacao" id="tipoOrdenacao" value="DESC">Decrescente
                                <input type="radio" name="tipoOrdenacao" id="tipoOrdenacao" value="ASC">Ascendente-->
                            </div>
                            
                        </div>
                                 </div>
                        <div class="text-right">
                            <!-- button id="bt_pdf" title="Exportar PDF" class="btn btn-success" type="button"><span
                                    class="glyphicon glyphicon-file"></span> Exportar PDF
                            </button -->
                        	<button id="bt_limpar"
                                    title="Limpar pesquisa" class="btn btn-warning" type="button"><span
                                    class="glyphicon glyphicon-remove"></span> Limpar
                            </button>
                            <button id="bt_pesquisar" title="Pesquisar" class="btn btn-primary" type="button"><span
                                    class="glyphicon glyphicon-search"></span> Pesquisar
                            </button>
                             <button id="bt_pdf" title="Exportar PDF" class="btn btn-success" type="button"><span
                                    class="glyphicon glyphicon-file"></span> Exportar PDF
                             </button>
                        <div class="col-md-1"></div>
                </fieldset>
            </div>
        </div>

        <br>
    </form>

    <div class="col-lg-12">
        <div class="page-header">
            <h1 id="forms">
                <!--                Dados da universidade --->
                <small>
                    Listagem
                </small>
            </h1>
        </div>
    </div>
    <div class="col-lg-12">
        <button class="bt-inserir btn btn-success"><span class="glyphicon glyphicon-plus"></span> Inserir</button>
    </div>
    <br/>

    <div class="row">
        <div class="col-md-12" id="container_listar">
            <?php $this->listarAction(); ?>
        </div>
    </div>
</div>
<script language="JavaScript">

    $('#form_search #prcid_orig').chosen();
    $('#form_search #prcid_dest').chosen();
    $('#form_search #esdid').chosen();
    $('#form_search #ordenacao').chosen();
    $('#form_search #dmddtentdocumento_search').datepicker();
    $('#form_search #dmddtemidocumento_search').datepicker();
    $('#form_search #dmdprazoemdata_1_search').datepicker();
    $('#form_search #dmdprazoemdata_2_search').datepicker();
    $('#form_search #dmdnumdocumento').mask('9999');


    var portaria = '<?php echo K_TIPO_DOCUMENTO_PORTARIA ?>';

    $('#form_search #tpdid').change(function(){
        mudarFormularioPesquisa();
    });

    /**
     * Limpa todos os campos, além de passar requisição para limpar $_POST.
     */
    $('#bt_limpar').click(function () {
            $('#esdid').val('').trigger('chosen:updated');
            $('#prcid_orig').val('').trigger('chosen:updated');
            $('#prcid_dest').val('').trigger('chosen:updated'); 
            $('#ordenacao').val('').trigger('chosen:updated');
            $('#tpdid').removeAttr('checked', true).parent('label').removeClass('active');
            $('#tpdid').prop('checked', false);
            $('input').prop('checked', false);
            $('input').val('');
            $('select option').removeAttr('selected');
            $('textarea').val('');
            $('#action').val('limpar');
            $('#controller').val('documento');
        //$('#form_search').trigger("reset");
        $('#form_search').ajaxSubmit();
        $('#action').val('listar');
        $('#form_search').ajaxSubmit({target: $('#container_listar').hide().fadeIn()});
    });
    /**
     * Exibe a listagem de acordo com os campos para pesquisa.
     */
    $('#bt_pesquisar').click(function () {
        $('#action').val('listar');
        $('#form_search').ajaxSubmit({target: $('#container_listar').hide().fadeIn()});
    });

    /**
     * Exibe uma modal para realizar cadastro.
     */
    $('.bt-inserir').click(function () {
        $.post(window.location.href, {'controller': 'documento', 'action': 'formulario'}, function (html) {
            $('#modal').html(html).modal('show');
        });
    });

    $('#bt_pdf').click(function() {
		$('#action').val('listarPdf');
		$('#form_search').submit();
        
        /*$.post(window.location.href, {'controller': 'documento', 'action': 'listarPdf'}, function (html) {
            $('#modal').html(html).modal('show');
        });*/
    });
    
    mudarFormularioPesquisa();

    function mudarFormularioPesquisa()
    {
        var value = $('#form_search #tpdid:checked').val();

        if(value == portaria){
            $('#form_search #prcid_dest').closest('.form-group').hide();
            $('#form_search #dmdreferencia').closest('.form-group').hide();
            $('#form_search #dmdnumsidoc').closest('.form-group').hide();
            $('#form_search #dmdprazoemdias').closest('.form-group').hide();
            $('#form_search #dmdprazoemdata_1').closest('.form-group').hide();

            $('#form_search #dmddb').closest('.form-group').fadeIn();
            $('#form_search #dmddtemidocumento_search').closest('.form-group').fadeIn();
        } else {
            $('#form_search #prcid_dest').closest('.form-group').fadeIn();
            $('#form_search #dmdreferencia').closest('.form-group').fadeIn();
            $('#form_search #dmdnumsidoc').closest('.form-group').fadeIn();
            $('#form_search #dmdprazoemdias').closest('.form-group').fadeIn();
            $('#form_search #dmdprazoemdata_1').closest('.form-group').fadeIn();

            $('#form_search #dmddb').closest('.form-group').hide();
            $('#form_search #dmddtemidocumento_search').closest('.form-group').hide();
        }
    }

    function fecharModal() {
        $('#modal').modal('hide');
        var data = {controller: 'documento', action: 'listar'};
        $.post(window.location.href, data, function (html) {
            $('#container_listar').hide().fadeIn().html(html);
        });
    }
    
    function manterLista(){
        $('#tpdid[value="<?php echo $_POST['tpdid'];?>"]').attr('checked', true).parent('label').addClass('active');//Tipo
        $('#dmdnumdocumento').val("<?php echo $_POST['dmdnumdocumento'];?>");//Número
        $('#esdid').val("<?php echo $_POST['esdid'];?>").trigger('chosen:updated');// Situação
        $('#form_search #dmddtentdocumento_search').datepicker().val("<?php echo $_POST['dmddtentdocumento'];?>");//Data do documento
        $('#form_search #dmddtemidocumento_search').datepicker().val("<?php echo $_POST['dmddtemidocumento'];?>");//Data do documento
        $('#prcid_orig').val("<?php echo $_POST['prcid_orig'];?>").trigger('chosen:updated');// Interessado
        $('#prcid_dest').val("<?php echo $_POST['prcid_dest'];?>").trigger('chosen:updated'); // Destino
        $('#dmdreferencia').val("<?php echo $_POST['dmdreferencia'];?>"); // Referência
        $('#dmdnumsidoc').val("<?php echo $_POST['dmdnumsidoc'];?>"); // Sidoc
        $('#form_search #dmdprazoemdata_1_search').datepicker().val("<?php echo $_POST['dmdprazoemdata_begin'];?>"); //Prazo em data
        $('#form_search #dmdprazoemdata_2_search').datepicker().val("<?php echo $_POST['dmdprazoemdata_end'];?>"); //Prazo em data
        $('#ordenacao').val("<?php echo $_POST['ordenacao'];?>").trigger('chosen:updated');//Ordenação
        $('#tipoOrdenacao[value="<?php echo $_POST['tipoOrdenacao'];?>"]').attr('checked', true).parent('label').addClass('active');//Tipo de Ordenação
        

        
    }

</script>
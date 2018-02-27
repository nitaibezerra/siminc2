<?php

    # Requisições
    include_once APPRAIZ. "acompanhamento/modulos/principal/monitorar-pi-request.inc";

    # Cabeçalho
    include APPRAIZ . "includes/cabecalho.inc";
    
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        
        <?php
        
            # Requisições
            include_once APPRAIZ. "acompanhamento/modulos/principal/monitorar-pi-cad.inc";
        
        ?>

    </div>
</div>

<script>
    $(function(){
        $('#claid').change(function(){
            $.ajax({
                url: '?modulo=principal/monitorar-ppa&acao=A&req=carregarMedida&claid=' + $(this).val(),
                dataType: 'json',
                success: function(retorno){
                    $('#span-medida').html(retorno.select);
                    if(retorno.medida == 't'){
                        $('.span-medida').show();
                    } else {
                        $('.span-medida').hide();
                    }
                }
            });
        });

        $('.selecionar-meta').click(function(){
            if(!$('#suoid').val()){
                swal('', 'Por favor, escolha uma unidade.', 'warning');
                return false;
            }
        });

        $('#suoid').change(function(){
            window.location.href = '?modulo=principal/monitorar-ppa&acao=A&suoid=' + $(this).val();
        });

        $('.div-meta').mouseenter(function(){
            var cor = $(this).data('cor');
            $(this).removeClass(cor).addClass('blue-bg');
        });

        $('.div-meta').mouseleave(function(){
            var cor = $(this).data('cor');
            $(this).removeClass('blue-bg').addClass(cor);
        });

        $('.detalhe-pi').click(function(){
            var url = $(this).attr('href');
            var title = $(this).attr('title');
            $('#detalhe-pi .modal-body').load(url);
            $('#detalhe-pi .modal-title').html(title);
            $('#detalhe-pi').modal();
            return false;
        });

        $('#btn-salvar').click(function(){
            if($('#span-medida').css('display') != 'none' && !$('#medidas').val()){
                swal('', "O campo 'Medidas a serem adotadas' é de preenchimento obrigatório", 'warning');
                return false;
            }

            $('#formulario').submit();
        });
        
        $('#btn_inserir_anexos').click(function(){
            abrirModalUpload();
        });

        // Evento de terminar de carregar arquivos
        Dropzone.options.formularioAnexo = {
            init: function() {
                
                this.on("success", function(file, response){
                    var jsonResponse = $.parseJSON(response);
                    inserirNovoAnexo(jsonResponse);
                });

                this.on("queuecomplete", function(file){

                    // Armazena o objeto Dropzone para chamar métodos
                    objFormularioAnexo = this;
                    // Chama mensagem de sucesso
                    swal({
                      title: "",
                      text: "Arquivos salvos com sucesso!",
                      timer: 2000,
                      showConfirmButton: false,
                      type: "success"
                    }, function(){
                        // Fecha o swal alert
                        swal.close();
                        // limpa campo de upload
                        objFormularioAnexo.removeAllFiles();
                        // fecha modal após a seleção
                        $('#modal_upload').modal('hide');
                    });
                });
            }
        };
        
        $('#table_anexos').on('click', '.btnRemoverAnexos', function(){
            var id = $(this).attr('data-anexos');
            $('.tr_anexos_'+ id).remove();
        });

        $('#percentual-empenhado').liquidMeter({
            shape: 'circle',
            color: '#0088cc',
            background: '#F9F9F9',
            fontSize: '24px',
            fontWeight: '600',
            stroke: '#F2F2F2',
            textColor: '#333',
            liquidOpacity: 0.9,
            liquidPalette: ['#333'],
            speed: 3000,
        });
    });
    
    function abrirModalUpload() {
        $('.modal_dialog_upload').show();
        $('#modal_upload').modal();
        $('#modal_upload .chosen-container').css('width', '100%');
    }

    function inserirNovoAnexo(json){
        var trHtml = '<tr style="height: 30px;" class="tr_anexos_'+ json.arqid +'" >'
            + '                    <td style="text-align: left;"><a href="#" onclick="javascript:abrirArquivo('+ json.arqid+ '); return false;" >'+ json.arqnome +'</a></td>'
            + '                    <td style="text-align: left;">'+ json.arqdescricao +'</td>'
            + '                    <td style="text-align: center;">'
            + '                         <input type="hidden" value="'+ json.arqid +'" name="listaAnexos[]">'
            + '                         <span class="glyphicon glyphicon-trash btnRemoverAnexos link" title="Excluir o arquivo '+ json.arqnome+ '" data-anexos="'+ json.arqid +'" >'
            + '                    </td>'
            + '                </tr>'
        ;
        $('#table_anexos').append(trHtml);
    }
</script>
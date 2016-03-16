jQuery(document).ready(function () {
    jQuery('.cpf').hide();
    jQuery('.cnpj').hide();
    jQuery('.alteracao').hide();
    jQuery('.cultura').hide();
    jQuery('.identifica2').hide();
    jQuery('.tematica2').hide();
    jQuery('.indigena2').hide();

    if ('<?=$ttmid?>' == '11' || '<?=$ttmid?>' == '10' || '<?=$ttmid?>' == '1') {
        jQuery('.tematica2').show();
    }
    if ('<?=$tpiid?>' == '23') {
        jQuery('.identifica2').show();
    }
    if ('<?=$tieid?>' == '3') {
        jQuery('.indigena2').show();
    }

    if ('<?=$monocorreualteracaodaicp?>' == 't') {
        jQuery('.alteracao').show();
    }
    if ('<?=$monrepresentagrupo?>' == 't') {
        jQuery('.cultura').show();
    }
    if ('<?=$montipoiniciativa?>') {
        if ('<?=$montipoiniciativa?>' == 1) {
            jQuery('.cpf').show();
        } else if ('<?=$montipoiniciativa?>' == 2) {
            jQuery('.cnpj').show();
        }
    }
    jQuery('input[name="montipoiniciativa"]').click(function () {
// 		jQuery('.parceiro1').attr('checked', '');
        jQuery('.cpf').hide();
        jQuery('.cnpj').hide();
        jQuery('.parceiro').val('');
        if (jQuery(this).val() == '1') {
            jQuery('.cpf').show();
        }
        if (jQuery(this).val() == '2') {
            jQuery('.cnpj').show();
        }
    });

    jQuery('input[name="alteracao"]').click(function () {
        if (jQuery(this).val() == 't') {
            jQuery('.alteracao').show();
        } else {
            jQuery('.alteracao').hide();
        }
    });

    jQuery('input[name="monrepresentagrupo"]').click(function () {
        //alert('2');
        jQuery('.cultura').hide();
        if (jQuery(this).val() == 't') {
            jQuery('.cultura').show();
        } else {
            jQuery('.cultura').hide();
        }
    });

    jQuery('[name="tpiid"]').change(function () {
        jQuery('.identifica2').hide();
        if (jQuery(this).val() == '23') {
            jQuery('.identifica2').show();
        } else {
            jQuery('.identifica2').hide();
        }
    });

    jQuery('[name="ttmid"]').change(function () {
        jQuery('.tematica2').hide();
        if (jQuery(this).val() == '11' || jQuery(this).val() == '10' || jQuery(this).val() == '1') {
            jQuery('.tematica2').show();
        } else {
            jQuery('.tematica2').hide();
        }
    });

    jQuery('[name="tieid"]').change(function () {
        jQuery('.indigena2').hide();
        if (jQuery(this).val() == '3') {
            jQuery('.indigena2').show();
        } else {
            jQuery('.indigena2').hide();
        }
    });

    jQuery('[name="moncoordmesmodainscr"]').change(function () {
        if (jQuery(this).val() == 't') {
            jQuery('.monnomecoord').val('<?=$dadoscoordenador['
            nome
            ']?>'
        )
            ;
            jQuery('.moncpfcoord').val(mascaraglobal('###.###.###-##', '<?=$dadoscoordenador['
            cpf
            ']?>'
        ))
            ;
            jQuery('.montelcoord').val('<?=$dadoscoordenador['
            ddd
            '].$dadoscoordenador['
            telefone
            ']?>'
        )
            ;
            jQuery('.monemailcoord').val('<?=$dadoscoordenador['
            email
            ']?>'
        )
            ;
        } else {
            jQuery('.monnomecoord').val('');
            jQuery('.moncpfcoord').val('');
            jQuery('.montelcoord').val('');
            jQuery('.monemailcoord').val('');
        }
    });

    jQuery('.deletarArquivo').click(function () {
        jQuery('#requisicao').val('deletarArquivo');
        jQuery('#form1').submit();
    });

    jQuery('.inserirparceiro').click(function () {
        jQuery('#requisicao').val('inserirparceiro');
        jQuery('#form1').submit();
    });
    jQuery('.salvarorcamento').click(function () {
        if (jQuery('#atidatainicio').val() == '') {
            alert("Informe a Data Início");
            jQuery('#atidatainicio').focus();
            return false;
        }
        if (jQuery('#atidatafim').val() == '') {
            alert("Informe a Data Fim");
            jQuery('#atidatafim').focus();
            return false;
        }

        jQuery('#requisicao2').val('inserirorcamento');
        jQuery('#form2').submit();
    });
    jQuery('.salvaresporadica').click(function () {
        if (jQuery('#atidatainicio2').val() == '') {
            alert("Informe a Data Início");
            jQuery('#atidatainicio2').focus();
            return false;
        }
        if (jQuery('#atidatafim2').val() == '') {
            alert("Informe a Data Fim");
            jQuery('#atidatafim2').focus();
            return false;
        }
        jQuery('#requisicao3').val('salvaresporadica');
        jQuery('#form3').submit();
    });

    jQuery('.soma1').blur(function () {
        var total = 0;
        var max = parseFloat(<?=$monvalorparcela?>);

        var materialconsumo = 0;
        if (Number(replaceAll(replaceAll(jQuery('[name="monaquisicaomaterialconsumo1"]').val(), ".", ""), ",", ".")) != '') {
            materialconsumo = parseFloat(replaceAll(replaceAll(jQuery('[name="monaquisicaomaterialconsumo1"]').val(), ".", ""), ",", "."));
            }
        var servicosculturais = 0;
        if (Number(replaceAll(replaceAll(jQuery('[name="moncontratacaoservicosculturais1"]').val(), ".", ""), ",", ".")) != '') {
            servicosculturais = Number(replaceAll(replaceAll(jQuery('[name="moncontratacaoservicosculturais1"]').val(), ".", ""), ",", "."));
            }
        var servicosdiversos = 0;
        if (Number(replaceAll(replaceAll(jQuery('[name="moncontratacaoservicosdiversos1"]').val(), ".", ""), ",", ".")) != '') {
            servicosdiversos = Number(replaceAll(replaceAll(jQuery('[name="moncontratacaoservicosdiversos1"]').val(), ".", ""), ",", "."));
            }
        var instrumentos = 0;
        if (Number(replaceAll(replaceAll(jQuery('[name="monlocacaodeinstrumentos1"]').val(), ".", ""), ",", ".")) != '') {
            instrumentos = Number(replaceAll(replaceAll(jQuery('[name="monlocacaodeinstrumentos1"]').val(), ".", ""), ",", "."));
            }
        var materiaispermanentes = 0;
        if (Number(replaceAll(replaceAll(jQuery('[name="monaquisicaomateriaispermanentes1"]').val(), ".", ""), ",", ".")) != '') {
            materiaispermanentes = Number(replaceAll(replaceAll(jQuery('[name="monaquisicaomateriaispermanentes1"]').val(), ".", ""), ",", "."));
            }

        saldo = max - (materialconsumo + servicosculturais + servicosdiversos + instrumentos + materiaispermanentes);

        if (Number(saldo)
            < 0) {
                //alert('O valor ultrapassou o máximo permitido');
                var valorAtual = parseFloat(replaceAll(replaceAll(jQuery(this).val(), ".", ""), ",", "."));
                saldo = saldo + valorAtual;
                alert('O valor do Saldo não pode ser menor que zero!');
                jQuery(this).val('');
                jQuery('[name="monsaldoorcamento1"]').val(mascaraglobal('###.###.###,##', saldo.toFixed(2)));
                return false;
                }

            jQuery('[name="monsaldoorcamento1"]').val(mascaraglobal('###.###.###,##', saldo.toFixed(2)));

            });

            jQuery('.soma2').blur(function () {
			var total = 0;
			var max = parseFloat(<?=$monvalorparcela?>);
			var materialconsumo = 0;
			if (Number(replaceAll(replaceAll(jQuery('[name="monaquisicaomaterialconsumo2"]').val(), ".", ""), ",", ".")) != '') {
                materialconsumo = parseFloat(replaceAll(replaceAll(jQuery('[name="monaquisicaomaterialconsumo2"]').val(), ".", ""), ",", "."));
                }
            var servicosculturais = 0;
            if (Number(replaceAll(replaceAll(jQuery('[name="moncontratacaoservicosculturais2"]').val(), ".", ""), ",", ".")) != '') {
                servicosculturais = Number(replaceAll(replaceAll(jQuery('[name="moncontratacaoservicosculturais2"]').val(), ".", ""), ",", "."));
                }
            var servicosdiversos = 0;
            if (Number(replaceAll(replaceAll(jQuery('[name="moncontratacaoservicosdiversos2"]').val(), ".", ""), ",", ".")) != '') {
                servicosdiversos = Number(replaceAll(replaceAll(jQuery('[name="moncontratacaoservicosdiversos2"]').val(), ".", ""), ",", "."));
                }
            var instrumentos = 0;
            if (Number(replaceAll(replaceAll(jQuery('[name="monlocacaodeinstrumentos2"]').val(), ".", ""), ",", ".")) != '') {
                instrumentos = Number(replaceAll(replaceAll(jQuery('[name="monlocacaodeinstrumentos2"]').val(), ".", ""), ",", "."));
                }
            var materiaispermanentes = 0;
            if (Number(replaceAll(replaceAll(jQuery('[name="monaquisicaomateriaispermanentes2"]').val(), ".", ""), ",", ".")) != '') {
                materiaispermanentes = Number(replaceAll(replaceAll(jQuery('[name="monaquisicaomateriaispermanentes2"]').val(), ".", ""), ",", "."));
                }
            saldo = materialconsumo + servicosculturais + servicosdiversos + instrumentos + materiaispermanentes;
            if (Number(saldo) > Number(max)) {
                alert('O valor ultrapassou o máximo permitido');
                jQuery(this).val('');
                return false;
                }
            jQuery('[name="monsaldoorcamento2"]').val(mascaraglobal('###.###.###,##', saldo.toFixed(2)));
            });
            });
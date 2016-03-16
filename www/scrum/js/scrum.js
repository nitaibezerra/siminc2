/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

/**
 * Inicializa as ações dos formulários do sistema.
 */
function inicializarFormulario()
{
    jQuery('#inserir, #atualizar').click(function() {
        var isValid = true;
        
        jQuery('#formPrincipal :input[required]').each(function() {
            var valorCampo = jQuery(this).val();
            valorCampo = valorCampo.trim();
            if (!valorCampo) {
                var nomeCampo = jQuery(this).parent().prev().text();
                
                // -- Tratamento especial para textarea, que fica dentro de uma div
                if (!nomeCampo) {
                    nomeCampo = jQuery(this).parent().parent().prev().text();
                }
                
                nomeCampo = nomeCampo.replace(':', '');
                alert('O campo "' + nomeCampo + '" não pode ser deixado em branco.');
                jQuery(this).parent().prev().parent().attr('class' , 'form-group has-error');
                jQuery(this).focus();
                isValid = false;
                return false;
            }
        });
        
        if(isValid){
            jQuery('#action').val('salvar');
            jQuery('#formPrincipal').submit();
        } else {
            return false;
        }
    });
    jQuery('#voltar').click(function(){
        jQuery('#action').val('voltar');
//        alert(jQuery('#action').val());
        jQuery('#formPrincipal').submit();
    });
    jQuery('#buscar').click(function(){
        jQuery('#action').val('filtrar');
        // -- Remove as valiações do formulário para filtrar a lista de programas
        jQuery('#formPrincipal :input[required]').each(function() {
            jQuery(this).removeAttr('required');
        });
        jQuery('#formPrincipal').submit();
    });
}

/**
 * Preenche o id do item e envia o formulário para carregar
 */
function carregarItem(id, item)
{
    jQuery('#' + item).val(id);
    jQuery('#action').val('carregar');
    jQuery('#formPrincipal').submit();
}

function carregaCombo(campo, campoBanco, id, itemEscopo, msgSelect)
{
    // -- Removendo os elementos da lista
    jQuery('#' + campo).find('option').remove().end()
        .append('<option>Carregando...</option>');
    // -- Requisitando ao servidor os novos elementos relacionados
    // -- à seleção anterior.
    jQuery.ajax({
        type:"POST",
        url:'scrum.php?modulo=principal/escopo/' + itemEscopo + '&acao=A',
        data:'action=json&' + campoBanco + '=' + id,
        async:false,
        success:function(data) {
            jQuery('#' + campo).find('option').remove().end()
                .append('<option>' + msgSelect + '</option>');
            if (data.error) {
                alert(data.error);
            } else {
                if (!data.options) {
                    return;
                }
                var options = data.options;
                for (pos in options) {
                    if (typeof(options[pos]) === 'function') {
                        continue;
                    }
                    jQuery('#' + campo).append(
                        '<option value="' + options[pos].codigo + '">' + options[pos].descricao + '</option>'
                    );
                }
            }
        }
    });
}

/**
 * Carrega dados de subprograma e atualiza o combo de subprogramas.
 * Utiliza o prgid para selecionar os subprogramas relacionados ao
 * programa selecionado no combo anterior.
 * 
 * @param prgid ID do programa.
 */
function carregaComboSubprograma(prgid)
{
    if (jQuery('#estid')[0]) {
        jQuery('#estid').find('option').remove().end()
            .append('<option>Selecione uma estória</option>');
    }
    carregaCombo('subprgid', 'prgid', prgid, 'subprograma', 'Selecione um subprograma');
}

/**
 * Carrega dados de estória e atualiza o combo de estórias.
 * Utiliza o subprgid para selecionar as estórias relacionadas ao
 * subprograma selecionado no combo anterior.
 * 
 * @param subprgid ID do programa.
 */
function carregaComboEstoria(subprgid)
{
    carregaCombo('estid', 'subprgid', subprgid, 'estoria', 'Selecione uma estória');
}
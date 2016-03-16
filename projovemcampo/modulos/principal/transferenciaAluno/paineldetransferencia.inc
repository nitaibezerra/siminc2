<?php

monta_titulo('','Dados da Transferência');
$_SESSION['projovemcampo']['modalidade'] = '';

$bloq = 'S'

?>

<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script language="javascript" type="text/javascript">
jQuery(document).ready(function() {
    
    jQuery('.pesquisar').click(function(){
        jQuery('#entid  option').attr('selected',true);
        jQuery('#turid  option').attr('selected',true);
        jQuery('#shtid  option').attr('selected',true);
        jQuery('#estcpf option').attr('selected',true);
        jQuery('#requisicao').val('');
        jQuery('#form').submit();
    });
    
    jQuery('.detalhe').live('click',function(){
        var traid = jQuery(this).attr('id');

        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: "&requisicao=mostraDetalheTransferencia&traid="+traid,
            async: false,
            success: function(msg){

                jQuery( "#dialog-detalhe" ).html(msg);       
                jQuery( "#dialog-detalhe" ).dialog({
                    resizable: false,
                    height:500,
                    width:800,
                    modal: true,
                    show: { effect: 'drop', direction: "up" },
                    buttons: {
                        "OK": function() {
                            jQuery( this ).dialog( "close" );                            
                        }
                    }
                });
            }
        });
    });
    
    jQuery('.baixar').click(function(){
        jQuery('#arqid').val(jQuery(this).attr('id'));
        jQuery('#form').submit();
    });
    
});

function filtraTurma(){
    jQuery('#requisicao').val('buscarComboTurma');
    jQuery('#entid option').attr('selected',true);
    jQuery.ajax({
        type: "POST",
        url: window.location.href,
        data: "&"+jQuery('#form').serialize(),
        async: false,
        success: function(msg){
            jQuery('#tr_turid').remove();
            jQuery('#tr_entid').after(msg);
        }
    });
}
    
    
/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
    var div_on = document.getElementById( campo + '_campo_on' );
    var div_off = document.getElementById( campo + '_campo_off' );
    var input = document.getElementById( campo + '_campo_flag' );
    if ( div_on.style.display == 'none' )
    {
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        input.value = '1';
    }
    else
    {
        div_on.style.display = 'none';
        div_off.style.display = 'block';
        input.value = '0';
    }
}

</script>
<style>
.div_muda{
align:top;
}
</style>
<div id="dialog-detalhe"></div>
<form id="form" name="form" method="POST" >
    <input type="hidden" value="" name="requisicao" id="requisicao" />
    <input type="hidden" value="" name="arqid" id="arqid" />
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center" border="0">
               <tr>
            <td class="SubTituloCentro" width="50%" colspan="2">Filtros</td>
        </tr>
        <tr>
            <td colspan="2">
                <table width="100%"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center" border="0">
                    <?
                    buscarComboEscola();
                    
                    buscarComboTurma();
                           
                    $sql = "SELECT
                                shtid as codigo,
                                shtdescricao as descricao
                            FROM
                                projovemcampo.statushistorictransferencia
                            ORDER BY
                                codigo";
                    
                    mostrarComboPopup( 'Status:', 'shtid',  $sql, '', 'Selecione o(s) Status' );
                    ?>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" width="25%">CPF:
                        </td>
                        <td width="75%">
                            <? echo campo_texto('estcpf', "N","S", "CPF", 16, 14, "", "", '', '', 0, 'id="estcpf" required', '', '', ');' ); ?>
                        </td>
                    </tr>
                </table>
            </td>
        <tr>
            <td class="SubTituloCentro" colspan="4">
                <input type="button" class="pesquisar" value="Pesquisar">
            </td>
        </tr>
    </table>
</form>
<?
monta_titulo('Alunos a transferir','');

$whereD = Array("apcO.apcid = {$_SESSION['projovemcampo']['apcid']}");
$whereO = Array("apcD.apcid = {$_SESSION['projovemcampo']['apcid']}");

extract($_POST);
// ver($estcpf,d);
if($estcpf[0] != ''){
   $cpfA = "AND est.estcpf = '{$estcpf}'"; 
   $cpfB = "AND est.estcpf = '{$estcpf}'"; 
}

if( $entid[0] != '' ){
    $whereD = Array("turO.entid IN (".implode(', ',$entid).")");
    $whereO = Array("turD.entid IN (".implode(', ',$entid).")");
}

if( $turid[0] != '' ){
    $whereD = Array("tra.turid_origem IN (".implode(', ',$turid).")");
    $whereO = Array("tra.turid_destino IN (".implode(', ',$turid).")");
}

if( $shtid[0] != '' ){
    $whereD = Array("sht.shtid IN (".implode(', ',$shtid).")");
    $whereO = Array("sht.shtid IN (".implode(', ',$shtid).")");
}

$sql = "SELECT DISTINCT
			shtdescricao as status,
			est.estcpf as cpf,
			est.estnome as nome,
			CASE 
				WHEN apcO.apcesfera = 'E'
				THEN munO.estuf
				ELSE munO.estuf|| ' - '||munO.mundescricao
			END as localorigem,
			entO.entnome as escolaorigem,
			turO.turdescricao as turmaorigem,
			aprO.apranoreferencia as anoorigem,
			CASE 
				WHEN apcD.apcesfera = 'E' OR apcD2.apcesfera = 'E'
				THEN coalesce(munD.estuf,munD2.estuf)
				ELSE coalesce(munD.estuf|| ' - '||munD.mundescricao,munD2.estuf|| ' - '||munD2.mundescricao)
			END as localdestino,
			coalesce(entD.entnome,entD2.entnome) as escoladestino,
			coalesce(turD.turdescricao,turD2.turdescricao) as turmadestino,
			coalesce(aprD.apranoreferencia,aprD2.apranoreferencia) as anodestino,
            CASE 
				WHEN arqt.arqid IS NULL
				THEN 'Não Existente'
				ELSE '<center><a id='||arqt.arqid||' style=\"cursor:pointer;color:blue\" class=baixar>Baixar</a></center>'
			END as arquivo,
            '<div class=detalhe id='|| tra.traid ||' >
                <img style=cursor:pointer width=55px src=/imagens/gadget_busca.png border=0 />
            </div>' as detalhe
		FROM
			projovemcampo.transferencia tra
        LEFT  JOIN projovemcampo.arquivo_tranferencia		arqt ON arqt.traid = tra.traid
		INNER JOIN projovemcampo.estudante est ON est.estid = tra.estid
		INNER JOIN projovemcampo.historico_transferencia       hst  ON hst.htrid   = tra.htrid_ultimo
		INNER JOIN projovemcampo.statushistorictransferencia   sht  ON sht.shtid   = hst.shtid_status
		-- Dados Origem
		INNER JOIN projovemcampo.turma turO ON turO.turid = tra.turid_origem
		INNER JOIN projovemcampo.adesaoprojovemcampo apcO ON apcO.secaid = turO.secaid
		INNER JOIN projovemcampo.anoprograma aprO ON aprO.aprid = apcO.aprid
		INNER JOIN entidade.entidade entO ON entO.entid = turO.entid
		INNER JOIN entidade.endereco endeO ON endeO.entid = entO.entid
		INNER JOIN territorios.municipio munO ON munO.muncod = endeO.muncod
		--Dados Destino
		LEFT JOIN entidade.entidade entD ON entD.entid = tra.entid_destino
		LEFT JOIN projovemcampo.turma turD ON turD.entid = entD.entid
		LEFT JOIN projovemcampo.adesaoprojovemcampo apcD ON apcD.secaid = turD.secaid
		LEFT JOIN entidade.endereco endeD ON endeD.entid = entD.entid
		LEFT JOIN projovemcampo.anoprograma aprD ON aprD.aprid = apcD.aprid
		LEFT JOIN territorios.municipio munD ON munD.muncod = endeD.muncod
		--Dados Destino 2
		LEFT JOIN projovemcampo.turma turD2 ON turD2.turid = tra.turid_destino
		LEFT JOIN entidade.entidade entD2 ON entD2.entid = turD2.entid
		LEFT JOIN projovemcampo.adesaoprojovemcampo apcD2 ON apcD2.secaid = turD2.secaid
		LEFT JOIN entidade.endereco endeD2 ON endeD2.entid = entD2.entid
		LEFT JOIN projovemcampo.anoprograma aprD2 ON aprD2.aprid = apcD2.aprid
		LEFT JOIN territorios.municipio munD2 ON munD2.muncod = endeD2.muncod
        WHERE   
            (
                shtid_status IN (2,3)  $cpfA AND ".implode(" AND ",$whereD)."
            )
            OR
            (
                ".implode(" AND ",$whereO)." $cpfB 
            )
        ORDER BY
            nome";  
$cabecalho = array("Status","CPF Aluno","Aluno","Local Origem","Escola Origem","Turma Origem","Ano Origem","Local Destino",
                    "Escola Destino","Turma Destino","Ano destino","Arquivo Anexo","Detalhe");
                    
$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', '', 'form_transferencias');
?>
 
<? registarUltimoAcesso(); ?>

<?php
header('Content-Type: text/html; charset=iso-8859-1');

$perfis         = pegaPerfilGeral();


$entid = $_REQUEST['entid'];
if($_SESSION['projovemcampo']['estuf']){
	$inner = "	INNER JOIN territorios.estado e ON e.estuf = mun.estuf
				INNER JOIN projovemcampo.adesaoprojovemcampo apc on apc.apccodibge::character(2) = e.estcod and apcesfera='E' ";
}elseif($_SESSION['projovemcampo']['muncod']){
	$inner = "INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.apccodibge::character(7) = mun.muncod and apcesfera in ('M')";
}
$sql = "SELECT DISTINCT 
			mun.muncod as muncod, 
			ede.estuf as estuf, 
			mun.mundescricao as mundescricao, 
			'Banco do Brasil' as agencia, 
			tur.entid as entid
		FROM 
			projovemcampo.turma tur
		INNER JOIN entidade.entidade ent ON ent.entid = tur.entid
		INNER JOIN entidade.endereco ede ON ede.entid = ent.entid
		INNER JOIN territorios.municipio mun ON mun.muncod = ede.muncod
		$inner		
		WHERE 
			tur.turstatus = 'A'
		AND apc.apcid = {$_SESSION['projovemcampo']['apcid']}
		AND tur.entid 	={$entid}";

$dadosAgencia = $db->pegaLinha($sql);
if(!in_array(PFL_COORDENADOR_ESTADUAL, $perfis)&&!in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)&&!in_array(PFL_ADMINISTRADOR, $perfis)&&!in_array(PFL_SUPER_USUARIO, $perfis)){
	echo "<script>
                    setInterval(function(){
                            var inputs = document.getElementsByTagName('input');
                            var selects = document.getElementsByTagName('select');

                            for(var i=0; i<selects.length; i++){
                                selects[i].disabled = true;
                             }

                            for(var i=0; i<inputs.length; i++){
                                inputs[i].disabled = true;
                             }

                            document.getElementById('btnVincularAgencia').disabled = 1
                    },100);
                  </script>
            ";
}
?>
<form name="formulario" id="form_listar_contratos" method="post" action="" >
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td></td>
            <td class="SubtituloEsquerda">Agência/Núcleo</td>
        </tr>
        <tr>
            <td></td>
            <td class="normal">Escolha uma agência para víncular ao núcleo
                escolhido</td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Estado</td>
            <td><input class="normal" type="text"
                       value="<?php echo( $dadosAgencia['estuf'] ); ?>" readonly="readonly"
                       id="estuf" />
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Município</td>
            <td><input class="normal" type="text"
                       value="<?php echo( $dadosAgencia['mundescricao'] ); ?>"
                       readonly="readonly" id="munname" />
            </td>
        </tr>
        <tr>

            <td class="SubtituloDireita" width="25%">Código Ibge</td>
            <td><input class="normal" type="text"
                       value="<?php echo( $dadosAgencia['muncod']); ?>"
                       readonly="readonly" id="muncod" />
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Banco</td>
            <td><input class="normal" type="text" value="Banco do Brasil"
                       readonly="readonly" id="munbanco" />
            </td>
        </tr>

        <tr>
            <td class="SubtituloDireita" width="25%">Raio</td>
            <td><select id="uraiokm" value="100">
                    <option value="">Selecione</option>
                    <option value="1">1 Km</option>
                    <option value="5">5 Km</option>
                    <option value="10">10 Km</option>
                    <option value="20">20 Km</option>
                    <option value="30">30 Km</option>
                    <option value="40">40 Km</option>
                    <option value="50">50 Km</option>
                    <option value="60">60 Km</option>
                    <option value="70">70 Km</option>
                    <option value="80">80 Km</option>
                    <option value="90">90 Km</option>
                    <option value="100">100 Km</option>
                    <option value="200">200 Km</option>
                    <option value="500">500 Km</option>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Agência</td>
            <td>
                <select id="agbcod" name="agbcod">
                    <option value="">Selecione o raio para busca</option>
                </select>
            </td>
        </tr>
        <tr>
            <td></td>
            <td>&nbsp;
                <input class="normal" type="button" value="Vincular Agência" id="btnVincularAgencia" name="btnVincularAgencia" />
                <input type="hidden" id="entid" value="<?php echo( $dadosAgencia['entid'] ); ?>" name="entid" />
                <input type="hidden" id="entid" value="<?php echo( $dadosAgencia['muncod'] ); ?>" name="entid" />

            </td>
        </tr>
    </table>
</form>

<div id="mensagens"></div>

<script type="text/javascript">
    $(document).ready(function(){
        $('#uraiokm').change(function(){

            var params = {};
			
            params['muncod'] 	= $('#muncod').val();
            params['estuf'] 	= $('#estuf').val();
            params['mundescricao'] 	= $('#mundsc').val();
            params['agencia'] 	= $('#agencia').val();
            params['uraiokm'] 	= $('#uraiokm').val();
            params['acao']  	= 'listaAgencias';

            if( params['uraiokm'] != '')
            {
                $.post( 'geral/ajax.php', params, function(response){

                    if( response.length == 0 )
                    {
                        alert('Não foi encontrada nenhuma agência com o raio informado');
                        return ;
                    }

                    $('#agbcod').html('');

                    jQuery.each( response, function( idx, el){
                                            
                        $('#agbcod').append('<option value="'+ el.agencia_dv +'">'+ el.co_agencia + ' / ' + el.no_agencia +'</option>');
                    } );

                }, 'json' );
            }
	        

        });

        $('#btnVincularAgencia').live('click',function(){

            var agbcod  = $('#agbcod').val();
            var muncod  = $('#muncod').val();
            var entid   = $('#entid').val();
            var uraiokm = $('#uraiokm').val();
            //var agbnom  = $('#agbcod').val();
            //var agbnom  = $('#agbcod:selected').node();
            //console.log(agbcod)
                        
            var params = {}
				
            if( agbcod == '' )
            {
                alert('Selecione a agência.');
                return;
            }
                        
            if( uraiokm == '' )
            {
                alert('Selecione um Raio.');
                return;
            }

            params['agbcod'] 	= agbcod;
            params['muncod'] 	= muncod;
            params['entid'] 	= entid;
            //params['agbnom']        = agbnom;
                       
                        
            params['acao'] 		= 'vincularAgenciaEscola';

            $.post( 'geral/ajax.php', params, function(response){
                if(response.status){
                    alert( response.retorno );
                    $( "#modalAgencia" ).dialog('close');
                };
            }, 'json');
                        
			
        });
		
    });
</script>

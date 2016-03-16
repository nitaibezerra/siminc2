<?php
header('Content-Type: text/html; charset=iso-8859-1');

$polomunicipio  = buscaPolos($_SESSION['projovemurbano']['pjuid']);
$perfis         = pegaPerfilGeral();


$nucid = $_REQUEST['nucid'];

if ($_SESSION['projovemurbano']['estuf']) {
    $sql = "SELECT DISTINCT 
            pmun.muncod
            , ede.estuf
            , pmun.mundsc
            , 'Banco do Brasil' as agencia
            , nuc.nucid
        FROM projovemurbano.nucleo nuc
        INNER JOIN projovemurbano.municipio m
                ON m.munid = nuc.munid
        INNER join projovemurbano.turma tur
                ON nuc.nucid = tur.nucid
        LEFT OUTER JOIN projovemurbano.associamucipiopolo amp
                ON amp.munid = m.munid
        LEFT OUTER JOIN projovemurbano.nucleoagenciabancaria nab
                ON nab.nucid = nuc.nucid
        LEFT OUTER JOIN projovemurbano.polo pol
                ON pol.polid = amp.polid
        LEFT OUTER JOIN projovemurbano.polomunicipio plm
                ON plm.pmuid = pol.pmuid
        LEFT OUTER JOIN entidade.entidade ent
                ON ent.entid = tur.entid
        LEFT OUTER JOIN entidade.endereco ede
                ON ede.entid = ent.entid
        LEFT OUTER JOIN public.municipio pmun
                ON pmun.muncod = ede.muncod
        WHERE ede.estuf = '" . $_SESSION['projovemurbano']['estuf'] . "'
        --AND plm.pjuid 	= " . $_SESSION['projovemurbano']['pjuid'] . "
        AND  nuc.nucstatus = 'A'
        AND nuc.nucid 	= {$nucid}";
    // ver($sql);
}
if ($_SESSION['projovemurbano']['muncod']) {

    $sql = "select distinct
		   
            m.estuf, m.mundescricao,m.muncod, nuc.nucid  
		 from territorios.municipio m 
		 
		inner join projovemurbano.cargameta c on c.cmecodibge::character(7) = m.muncod and cmetipo in('C','M') 
                INNER JOIN projovemurbano.municipio mun
                ON mun.muncod = m.muncod
                INNER JOIN projovemurbano.nucleo nuc
                ON mun.munid = nuc.munid
		left join projovemurbano.projovemurbano p on p.muncod = c.cmecodibge::character(7) 
		left join workflow.documento dd on dd.docid = p.docid 
		left join workflow.historicodocumento ht on ht.hstid = dd.hstid
		left join workflow.estadodocumento esd on esd.esdid = dd.esdid                   
        
                     WHERE m.muncod = '" . $_SESSION['projovemurbano']['muncod'] . "'
                AND nuc.nucid 	= {$nucid}
        
        ";





    //ver($db->carregar($sql));
}
$dadosAgencia = $db->pegaLinha($sql);
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
                       value="<?php if ($_SESSION['projovemurbano']['muncod']) {
    echo( $dadosAgencia['mundescricao'] );
} else echo( $dadosAgencia['mundsc'] ); ?>"
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
                <?php if(!in_array(PFL_CONSULTA, $perfis)){ ?>
                <input class="normal" type="button" value="Vincular Agência" id="btnVincularAgencia" name="btnVincularAgencia" />
                <?php } ?>
                <input type="hidden" id="nucid" value="<?php echo( $dadosAgencia['nucid'] ); ?>" name="nucid" />
                <input type="hidden" id="nucid" value="<?php echo( $dadosAgencia['muncod'] ); ?>" name="nucid" />

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
            params['mundsc'] 	= $('#mundsc').val();
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
            var nucid   = $('#nucid').val();
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
            params['nucid'] 	= nucid;
            //params['agbnom']        = agbnom;
                       
                        
            params['acao'] 		= 'vincularAgenciaNucleo';

            $.post( 'geral/ajax.php', params, function(response){
                if(response.status){
                    alert( response.retorno );
                    $( "#modalAgencia" ).dialog('close');
                };
            }, 'json');
                        
			
        });
		
    });
</script>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
    <tr>
        <!--<td rowspan="10" align="center" valign="top" width="1">
            Tramitação
        </td>-->
        <td rowspan="10" align="center" valign="top" width="1">
            <?php
            	// Barra de estado WORKFLOW
                if( possui_perfil(PFLCOD_CALL_CENTER)){
                    wf_desenhaBarraNavegacao( $docid, array('smiid' =>  $smiid), array('acoes' => true));
                }else if($docid){
                    wf_desenhaBarraNavegacao($docid, array('smiid' =>  $smiid));
            	}
            ?>
            </td>
    </tr>
</table>

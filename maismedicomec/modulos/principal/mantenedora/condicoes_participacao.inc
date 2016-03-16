<?PHP

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Mais Médicos - MEC', 'Condições de Participação' );

    $abacod_tela    = ABA_CADASTRO_MANTENEDORA;
    $url            = 'maismedicomec.php?modulo=principal/mantenedora/condicoes_participacao&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);
    
	if($_SESSION['maismedicomec']['mntid'] == ''){
        $mntid = $_REQUEST['mntid'];

        if( $_SESSION['maismedicomec']['mntid'] == '' ){
            $_SESSION['maismedicomec']['mntid'] = $mntid;
        }
    }else{
        $mntid = $_SESSION['maismedicomec']['mntid'];
    }

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" language="javascript">
</script>

<form id="formulario" method="post" name="formulario" >
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
	    <tr>
	        <td height="25"></td>
	    </tr>
	    <tr>
	        <td>
                <?php if($hab_maismedicos == 'N'){?>
                	<p style="margin-left: 38%; text-decoration: underline; font-size: 14px; font-weight: bold; color:#FF0000;"><b>MUNICÍPIO NÃO ATENDE AS CONDIÇÕES DE PARTICIPAÇÃO<b></p>
                <?php } ?>
	            <span style="margin-left: 3%; text-decoration: underline; font-size: 14px; font-weight: bold;">
	                CONDIÇÕES DE PARTICIPAÇÃO
	            </span>
	            
	            <br><br>
	            
	            <p style="margin-left: 5%; text-align: justify; font-size: 13px; font-weight: bold;">
	                Poderá participar desta chamada pública mantenedora de IES legalmente constituída no País que tenha, pelo menos, 1 (uma) mantida já credenciada integrante do Sistema Federal de Ensino e com cadastro no Sistema e-MEC até a data de publicação do presente Edital.
	            </p>
	            
	            <p style="margin-left: 5%; text-align: justify; font-size: 13px; font-weight: bold;">
	                A mantenedora deverá indicar a mantida que será responsável pela oferta do curso de graduação em medicina ou propor o credenciamento de IES ou de campus fora de sede, nos termos dos itens 3.1.1, 3.1.2 e 3.1.3 do Edital. 
	            </p>

	            <p style="margin-left: 5%; text-align: justify; font-size: 13px; font-weight: bold;">
	                As mantenedoras poderão apresentar propostas que contemplem uma das opções abaixo: 
	            </p>
	            
	            <ul style="list-style-type:disc;margin-left: 5%; font-size: 13px;">
				  <li>Autorização de curso de graduação em medicina para mantida que detenha autorização de funcionamento no município para o qual está concorrendo;</li>
				  <li>Credenciamento de nova mantida no município para o qual está concorrendo e autorização de curso de graduação em medicina; </li>
				  <li>Credenciamento de campus fora de sede (no caso exclusivo de universidades credenciadas na Unidade da Federação do município para o qual está concorrendo) e autorização de curso de graduação em medicina.</li>
				</ul>
				
				<p style="margin-left: 5%; text-align: justify; font-size: 13px; font-weight: bold;">
	                Não poderão participar deste processo de seleção:  
	            </p>
	            
	            <ul style="list-style-type:disc;margin-left: 5%; font-size: 13px;">
				  <li>Consórcio de mantidas; </li>
				  <li>A mantenedora que teve processo de credenciamento de Instituição de Educação Superior ou de autorização de curso de graduação em medicina indeferidos nos últimos dois anos, a contar da data de publicação do ato de indeferimento.</li>
				</ul>

	        </td>
	    </tr>
	    <!-- 
	    <tr>
	        <td align="center" bgcolor="#CCCCCC" colspan="2">
                <?php if( $hab_maismedicos == 'S' ){ ?>
                <input type="submit" value="Continuar" id="btnContinuar"/>
                <input type="button" value="Cancelar" id="btnCancelar" onclick="cancelar();"/>
                <?php } else { ?>
                <input type="button" value="OK" id="btnCancelar" onclick="cancelar();"/>
                <?php } ?>
	        </td>
	    </tr>
	     -->						
	</table>
</form>



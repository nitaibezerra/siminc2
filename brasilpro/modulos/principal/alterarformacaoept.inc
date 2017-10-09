<?php

include_once( APPRAIZ."includes/classes/dateTime.inc" );

cte_verificaSessao();

if (!array_key_exists('entid', $_REQUEST)) {
    echo '<script type="text/javascript">'
        .'alert("O código da escola não foi informado.");'
        .'history.back();'
        .'</script>';
} else {
	if ($_REQUEST['atualizar'] == 'gravarformacaoept') {
		
		$sql = "SELECT count(*) FROM entidade.entidadedetalhe WHERE entid = " .$_REQUEST['entid'];
        $cnt = (integer) $db->pegaUm($sql);
        
        $obData = new Data();
        
        $entdstatusept = isset( $_REQUEST["csStatusOferta"] ) ? "'{$_REQUEST["csStatusOferta"]}'" : 'null';
        $entddataprevisaoept = $_REQUEST["entddataprevisaoept"] ? "'". $obData->formataData( $_REQUEST["entddataprevisaoept"], "YYYY-MM-DD" ) ."'" : 'null';
        
        if ($cnt == 1) {
            $sql = '
                UPDATE
                    entidade.entidadedetalhe
                SET
                    entdoferta_ept 		= ' . $_REQUEST['entdoferta_ept'] .',
                    entdstatusept 		= ' . $entdstatusept .',
                    entddataprevisaoept = ' . $entddataprevisaoept .' 
                WHERE
                    entid = ' . $_REQUEST['entid'];
        } else {
            $sql = '
            SELECT
                entcodent
            FROM
                entidade.entidade
            WHERE
                entid = '. $_REQUEST['entid'];

            $entcodent = $db->pegaUm($sql);

            if ($entcodent) {
                $sql = '
                INSERT INTO entidade.entidadedetalhe
                (
                    entid,
                    entdoferta_ept,
                    entdstatusept,
                    entddataprevisaoept,
                    entcodent
                ) VALUES (
                    ' . $_REQUEST['entid'] . ', 
                    ' . $_REQUEST['entdoferta_ept'] . ',
                    ' . $entdstatusept . ',
                    ' . $entddataprevisaoept . ', 
                     \'' .trim($entcodent). '\')';
            }
        }

        $db->executar($sql);

        if ($_REQUEST['entdoferta_ept'] == 'false') {
            $sql   = '
            SELECT
                ippid,
                ipptitulo
            FROM
                cte.itensppp
            WHERE
                ippbloqdifept = true';

            $itens = (array) $db->carregar($sql);

            if ($itens) {
                foreach ($itens as $item) {
                    $sql = "SELECT cppid FROM cte.conteudoppp WHERE ippid = '". $item['ippid'] ."' AND entid = '". $_REQUEST['entid'] ."'";
                    $cpp = current((array) $db->carregar($sql));

                    if ($cpp['cppid']) {
                        $sql = "DELETE FROM cte.conteudopppcursotecnico WHERE cppid = '".$cpp['cppid']."'";
                        $db->executar($sql);
                        $sql = "DELETE FROM cte.conteudoppp WHERE cppid = '".$cpp['cppid']."'";
                        $db->executar($sql);
                    }
                }
            }
        }

        $db->commit();
        echo '<script type="text/javascript">alert("Formação EPT foi indicada com sucesso.");window.opener.location.reload();window.close();</script>';
        exit;
	}

	$sql = 'SELECT ent.entnome, entd.entdoferta_ept, entdstatusept, entddataprevisaoept 
			FROM entidade.entidade AS ent
    		INNER JOIN entidade.entidadedetalhe AS entd ON entd.entcodent = ent.entcodent
    		WHERE ent.entid = ' . $_REQUEST['entid'];
	
    $ent = (array) $db->pegaLinha($sql);
    // conforme conversa com douglas, caso a escola não tenha detalhes no join com entcodent,
    // tentar no entid
    if(!$ent[0]) {
		$sql = 'SELECT ent.entnome, entd.entdoferta_ept, entdstatusept, entddataprevisaoept 
				FROM entidade.entidade AS ent
    			INNER JOIN entidade.entidadedetalhe AS entd ON entd.entid = ent.entid
    			WHERE ent.entid = ' . $_REQUEST['entid'];
	
	    $ent = (array) $db->pegaLinha($sql);
    }
    $entddataprevisaoept = $ent["entddataprevisaoept"];
?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title><?php echo NOME_SISTEMA; ?></title>
		
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script src="../includes/calendario.js"></script>
		<script language="JavaScript">
			function setpfl() {
				document.setperfil.submit();
			}

			function abrirsistema( sisid ) {
				location.href = "../muda_sistema.php?sisid=" + sisid;
			}
			
			function abrir_popup_mensagem()
			{
				w = window.open( '../geral/popup_mensagem.php', 'mensagens', 'width=780,height=400,scrollbars=yes,menubar=no,toolbar=no,statusbar=no' );
				w.focus();
			}
			function validasubmit(form) {
				if(document.form.entdoferta_ept[0].checked || document.form.entdoferta_ept[1].checked) {
					document.form.submit();
					return true;
				} else {
					alert("Selecione alguma das opções.");
				}
			}
			
			function informarOfertaEPT(){
				var eptSim = document.getElementById('entdoferta_ept_sim');
				var eptNao = document.getElementById('entdoferta_ept_nao');
					
				if( eptSim.checked ){
					document.getElementById('naoinformado').style.display = 'none';
					document.getElementById('statusOferta').style.display = '';
				}
				else{
					if( eptNao.checked ){
						document.getElementById('naoinformado').style.display = '';
						document.getElementById('statusOferta').style.display = 'none';
						document.getElementById('funcionamento').checked = false;
						document.getElementById('previsto').checked = false;
						document.getElementById('divDtPrevisao').style.display = 'none';
						document.getElementById('entddataprevisaoept').value = "";
					}	
				}
			}
			
			function informarStatusOferta(){
				var funcionamento = document.getElementById('funcionamento');
				var previsto = document.getElementById('previsto');
					
				if( funcionamento.checked ){
					document.getElementById('divDtPrevisao').style.display = 'none';
					document.getElementById('entddataprevisaoept').value = "";
				}
				else{
					if( previsto.checked ){
						document.getElementById('divDtPrevisao').style.display = '';
					}
				}
			}
			
			
		</script>

		
		<script type="text/javascript" language="javascript" src="../includes/agrupador.js"></script>
		<style type="text/css">
			.combo {
				width: 200px;
			}
		</style>


		<script type="text/javascript" src="../includes/dtree/dtree.js"></script>
		<script type="text/javascript" src="../includes/prototype.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
	</head>
<form action="brasilpro.php?modulo=principal/alterarformacaoept&acao=A" name="form" method="post">
<input type="hidden" name="atualizar" value="gravarformacaoept">
<input type="hidden" name="entid" value="<? echo $_REQUEST['entid']; ?>">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <colgroup>
        <col/>
    </colgroup>
    <tbody>
        <tr>
            <td style="padding:15px; background-color:#fafafa; font-weight: bold; color:#404040; vertical-align: top;"><? echo $ent['entnome']; ?></td>
        </tr>
        <tr>
            <td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;">
            	Informe se a escola oferta ou ofertará EPT: 
            	<input type="radio" name="entdoferta_ept" id="entdoferta_ept_sim" 
            		onclick="informarOfertaEPT()" 
            		<? echo (($ent['entdoferta_ept'] == 't')?'checked':''); ?> value="true"> <strong>Sim</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            	<input type="radio" name="entdoferta_ept" id="entdoferta_ept_nao"
            		onclick="informarOfertaEPT();" 
            		<? echo (($ent['entdoferta_ept'] == 'f')?'checked':''); ?> value="false"> <strong>Não</strong>
            </td>
        </tr>

		<tr id="statusOferta" style="display: none;">
			<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;">
				<div>
					Por Favor, informe se a oferta EPT já está em funcionamento ou está previsto: <br />
					
					<input <?php echo (($ent['entdstatusept'] == 'F')?'checked':''); ?> type="radio" onclick="informarStatusOferta();" name="csStatusOferta" id="funcionamento" value="F" /><strong>Em Funcionamento</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input <?php echo (($ent['entdstatusept'] == 'P')?'checked':''); ?> type="radio" onclick="informarStatusOferta();" name="csStatusOferta" id="previsto" value="P" /><strong>Previsto</strong><br /><br /> 
					<div id="divDtPrevisao" style="display: none;">
						Por Favor, informe a data de previsão: <br />
						<?php 
						
						echo campo_data( 'entddataprevisaoept','N', 'S', '', 'S' ); ?>
					</div>
				</div>			
			</td>
        </tr>
        
        <tr>
            <td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;"><input type="button" name="Salvar" value="Salvar" onclick="return validasubmit();" /> <input type="button" name="Voltar" value="Voltar" onclick="window.close();" /></td>
        </tr>
    </tbody>

<?php
	$sql = "SELECT ippid, ipptitulo FROM cte.itensppp WHERE ippbloqdifept = true ORDER BY ippordem";
	$itens = (array) $db->carregar($sql);
    
	$html = "<tbody id=\"naoinformado\" style=\"display:none\">";
	$html .= "<tr><td style=\"padding:15px; background-color:red; font-weight: bold; color:#404040; vertical-align: top;\">Você está informando que a escola não possui formação EPT. Ao clicar no botão salvar, os seguintes campos terão os seus dados excluídos e bloqueados para cadastro.</td></tr>";	    
    if($itens) {
    	foreach($itens as $item) {
    		$html .= "<tr><td style=\"padding:15px; background-color:#fafafa; font-weight: bold; vertical-align: top;\">". $item['ipptitulo'] ."</td></tr>";

    		$sql = "SELECT cpptexto FROM cte.conteudoppp WHERE entid = '". $_REQUEST['entid'] ."' AND ippid = '". $item['ippid'] ."'";
			$respostas = current((array) $db->carregar($sql));

			$html .= "<tr><td style=\"padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;\">";
			if($respostas) {
				$html .= $respostas['cpptexto'];	
			} else {
				$html .= "Em branco";
			}
			$html .= "</td></tr>";
    	}
    	
    }        
    
    $html .= "</tbody>";
    
    echo $html;

?>

</table>
</form>
</html>

<?

}

?>

<script type="text/javascript">
	informarOfertaEPT();
	informarStatusOferta();
</script>
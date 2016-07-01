<?
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "evento/classes/CoendereCoentrega.class.inc";
 
$obCoendereCoentrega = new CoendereCoentrega($_GET['coeid']);
?>
	 <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER
        {
            background: none;
            position: absolute;
            width: 100%;
            text-align: center;
            z-index: 8000;
            height: 100%;
        }

        #loader {
            background-color: #fff;
            color: #000033;
            width: 300px;
            border: 2px solid #cccccc;
            font-size: 12px;
            padding: 25px;
            font-weight: bold;
            margin: 150px auto;
        }
    </style>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script type="text/javascript"><!--
    	function salvaEnd(){
    		document.formulario.submit();
    	}
    	
    	function excluirEnderecoEntrega(coeid){
			if(confirm('Deseja excluir registro')){
				window.location.href = "/evento/evento.php?modulo=principal/cadCompraEnd&acao=A&coeidExcluir="+coeid;
				return true;
			} else {
				return false;
			}
		}
		function fecharPopup(){
			return window.close();
		}
    	
    --></script>
<?php
//Chamada de programa 
monta_titulo( $titulo_modulo, '' );
montaCabecalhoUnidade($_SESSION['unidade']); 
montaCabecalhoProcesso($_SESSION['copid'], false); 
?>
<form method="post" name="formulario" id="formulario" action="/evento/evento.php?modulo=principal/cadCompraEnd&acao=A">
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="font-weight: bold" colspan="2">
					Dados de Entrega
			</td>
		</tr>
		<tr>
			<td width=25%  class="SubTituloDireita" >
					Nome do local de entrega:
			</td>
			<td id="tdEntdsc">
	 		<? echo $obCoendereCoentrega->coenddsc; ?> 
			<input type="hidden" name="formulario" id="formulario" value="1" />
			<input type="hidden" name="coeid" id="coeid" value="<? echo $obCoendereCoentrega->coeid; ?>" />
			<!-- input type="hidden" name="entid" id="entid" value="<? echo $arInstituicao['entid']; ?>" /-->
			</td>
		</tr> 
		<tr>
			<td class="SubTituloDireita" >
					Responsável pelo recebimento:
			</td>
			<td><?$coendrespalmx = $obCoendereCoentrega->coendrespalmx; ?>
				<? echo $coendrespalmx ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
					Horário de Funcionamento:
			</td>
			<td><?$coendhrini = $obCoendereCoentrega->coendhrini; ?>
				<? echo $coendhrini; ?>
				 às 
				 <?$coendhrfim = $obCoendereCoentrega->coendhrfim; ?>
				<? echo $coendhrfim; ?>
				horas
			</td>
		<tr>
	<?php
               
        $endereco = new Endereco();
        $entidade->enderecos[0] = $endereco;

        $mundescricao = "";
		if($obCoendereCoentrega->muncod){
           $mundescricao = $obCoendereCoentrega->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$obCoendereCoentrega->muncod}'");
        }
        echo ''
                . '<tr>'
                . '<td style="font-weight: bold" colspan="2">Endereço</td>'
                . '</tr>'

                . '<tr>'
                . '<td align="right" class="SubTituloDireita" style="width: 25%; white-space: nowrap"><label>CEP:</label></td>'
                . '<td>'
                .  $obCoendereCoentrega->coendcep  
                . '</td>'
                . '</tr>'

                . '<tr id="escolha_logradouro_id" style="display:none">'
                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Selecione o Logradouro:</label></td>'
                . '<td>'
                . (!$dados['coendlog']? $entidade->enderecos[0]->endlog : $dados['coendlog']) 
                . '</td>'
                . '</tr>'
                
                . '<tr>'
                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Logradouro:</label></td>'
                . '<td>'
                . $obCoendereCoentrega->coendlog  
                . '</td>'
                . '</tr>'

                . '<tr>'
                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>'
                . '<td>'
                . $obCoendereCoentrega->coendnum 
                . '</td>'
                . '</tr>'

                . '<tr>'
                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>'
                . '<td>'
                . $obCoendereCoentrega->coendcom 
                . '</td>'
                . '</tr>'

                . '<tr>'
                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>'
                . '<td>'
                . $obCoendereCoentrega->coendbai 
                . '</td>'
                . '</tr>'

                . '<tr>'
                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF: </label></td>'
                . '<td>'
                . $mundescricao . ' - '. $obCoendereCoentrega->estuf 
                . '</td>'
                . '</tr>';
                
	?>
	</table>
	<div id=buttonAcao>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none">
			<tr>
				<td class="SubTituloDireita" colspan="2" style="text-align:center">
					 <input type="button" name="btnFechar" value="Fechar" id="btnFechar" onclick="fecharPopup();">  
				</td>
			</tr>
	</table>
	</div>
</form>

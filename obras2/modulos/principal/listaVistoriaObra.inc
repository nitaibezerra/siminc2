<?php
// empreendimento || obra || orgao
verificaSessao( 'empreendimento' );

switch ( $_POST['operacao'] ){
	case 'apagar':
		$empreendimentoSupervisao = new EmpreendimentoSupervisao();
		$empreendimentoSupervisao->excluir( $_POST['emsid'] );
		$db->commit();
		die("<script>
				alert('Operação realizada com sucesso!');
				window.location = '?modulo=principal/listaVistoriaObra&acao=A';
			 </script>");
	case 'editar':
		$_SESSION['obras2']['emsid'] = $_POST['emsid'];
		die("<script>
				window.location = '?modulo=principal/cadVistoriaObra&acao=E';
			 </script>");
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$empid = $_SESSION['obras2']['empid'];
if( $empid ){
	$db->cria_aba(ID_ABA_EMP_CADASTRADO,$url,$parametros);
}else{
	$db->cria_aba(ID_ABA_CADASTRA_EMP,$url,$parametros);
}

$empreendimento = new Empreendimento( $empid );
$empreendimento->montaCabecalho();

monta_titulo( 'Lista de Vistoria da Obra', '' );

$empreendimentoSupervisao = new EmpreendimentoSupervisao();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<form name="formulario" id="formulario" method="post" action="">
    <input type="hidden" name="emsid" id="emsid" value=""/>
    <input type="hidden" name="operacao" id="operacao" value=""/>
</form>    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td>
            <?php
            	$param 			= array();
            	$param['empid'] = $empid;
            	$dados = $empreendimentoSupervisao->listaDados($param);
            ?>
            <table class="tabela" align="center" style="width: 98%;">
                <tr>
                <?php 
				$cabecalho = array( "Ação",
                                    "Data da Vistoria",
                                    "Previsão de Conclusão da Obra",
                                    "Data de Inclusão",
                                    "Situação da Obra",
                                    "Responsável",
                                    "Inserido Por");
                
                foreach($cabecalho as $titulos){ 
                ?>
                    <th><? echo  $titulos  ?></th>
                <? 
				}
				?>
                </tr>
                <?php
                $btnExc    = '<img src="/imagens/excluir_01.gif" style="cursor:pointer;" title="Não é possível excluir essa vistoria" border="0" onclick=" alert(\'Não é possível excluir essa vistoria\')"> ';
                $btnExcHab = '<img src="/imagens/excluir.gif" border="0" title="Excluir" style="cursor:pointer;" onclick="javascript:excluirVistoria( %s );">';
                $btnEdt    = '<img src="/imagens/alterar_pb.gif" style="cursor:pointer;" title="Não é possível editar essa vistoria" border="0" onclick=" alert(\'Não é possível editar essa vistoria\')"> ';
                $btnEdtHab = '<img src="/imagens/alterar.gif" border="0" title="Editar" style="cursor:pointer;" onclick="javascript:editarVistoria( %s );">';
                    
                $totReg = count( $dados );
				$i		= 1;
                foreach($dados as $chave){ 
                ?>
                <tr <? if( $i%2 ) print("bgcolor=#f0f0f0") ?>>
                    <td align="center">
                    <?php
					echo sprintf($btnEdtHab . "&nbsp;", $chave['emsid']);
					if ( (($chave["usucpf"] == $_SESSION['usucpf'] || possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR)) && $totReg == $i) ) /*&& empty($chave["supid_1"])*/ ){
						echo sprintf($btnExcHab, $chave['emsid']);
					}else{
//						echo $btnEdt . "&nbsp;";	
						echo $btnExc;	
					}
                    ?>
                    </td>
                    <td align="center">
                        <? print($chave["emsdtvistoria"]); ?>
                    </td>
                    <td align="center">
                        <? print($chave["emsdtconclusao"]); ?>
                    </td>
                    <td align="center">
                        <? print($chave["emsdtcadastro"]); ?>
                    </td>
                    <td>
                        <? print($chave["stadesc"]); ?>
                    </td>
                    <td>
                    	<img border="0" onclick='envia_email("<?php echo $chave["entcpf"] ?>");' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
                        <? print($chave["entnome"]); ?>
                    </td>
                    <td>
                    	<img border="0" onclick='envia_email("<?php echo $chave["usucpf"] ?>");' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
                        <? print($chave["usunome"]); ?>
                    </td>
                </tr>
                <?php 
                		$i++; 
					} 
				?>
            </table>
            </td>
        </tr>
        <tr bgcolor="#C0C0C0">
            <td align="left">
			<?php
            if ( $empreendimento->verificaEquivalenciaContratoCronogramaDoObjetoPorEmpid( $empid ) ){
            ?>
            	<input style="cursor:pointer; margin-left:15px;" type="button" name="inserir_vistoria" value="Inserir Nova Vistoria" onclick="window.location='?modulo=principal/cadVistoriaObra&acao=A'" />
			<?php
            }else{
            	echo '&nbsp;&nbsp;&nbsp;<font style="color: red;">O valor do <b>cronograma</b> deve ser igual ao valor do <b>contrato</b> em todos os <b>objetos</b> para liberar a <b>vistoria</b>.</font>';
            }
            ?>
            </td>
        </tr>        
</table>
<script type="text/javascript">
function excluirVistoria( emsid ){
    if(confirm("Deseja realmente excluir essa vistoria?")){
    	$('#emsid').val( emsid );
    	$('#operacao').val( 'apagar' );
    	$('#formulario').submit();
    }
}

function editarVistoria( emsid ){
    	$('#emsid').val( emsid );
    	$('#operacao').val( 'editar' );
    	$('#formulario').submit();
}
</script>
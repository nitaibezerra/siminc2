<?php
// empreendimento || obra || orgao
verificaSessao( 'empreendimento' );

unset($_SESSION['obras2']['sosid']);

$empid = $_SESSION['obras2']['empid'];

if ( empty( $empid ) ){
	die("<script>
			alert('Faltam parametros para acessar esta tela!');
			location.href = '?modulo=principal/listaEmpreendimentos&acao=A';
		 </script>");	
}

switch ( $_POST['operacao'] ){
	case 'apagar':
		$supervisaoEmpresa = new SupervisaoEmpresa();
		$supervisaoEmpresa->excluir( $_POST['sueid'] );
		$db->commit();
		die("<script>
				alert('Operação realizada com sucesso!');
				window.location = '?modulo=principal/listVistoriaEmp&acao=A';
			 </script>");
	case 'editar':
		$_SESSION['obras2']['sueid'] = $_POST['sueid'];
		die("<script>
				window.location = '?modulo=principal/cadVistoriaEmpresa&acao=E';
			 </script>");
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$db->cria_aba(ID_ABA_EMP_CADASTRADO,$url,$parametros);

$empreendimento = new Empreendimento( $empid );
$empreendimento->montaCabecalho();

monta_titulo( 'Lista de Supervisão', '' );

$supervisaoEmpresa = new SupervisaoEmpresa();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<form name="formulario" id="formulario" method="post" action="">
    <input type="hidden" name="sueid" id="sueid" value=""/>
    <input type="hidden" name="operacao" id="operacao" value=""/>
</form>    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td>
            <?php
            	$param 			= array();
            	$param['empid'] = $empid;
            	$dados 			= $supervisaoEmpresa->listaDados($param);
            ?>
            <table class="tabela" align="center" style="width: 98%;">
                <tr>
                <?php 
				$cabecalho = array( "Ação",
                                    "Nº OS",
                                    "Situação da Supervisão",
                                    "Data da Supervisão",
                                    "Data de Inclusão",
                                    "Responsável",
                                    "Cargo do Responsável",
                                    "Inserido Por",
									"Unidade em Funcionamento?");
                
                foreach($cabecalho as $titulos){ 
                ?>
                    <th><? echo  $titulos  ?></th>
                <? 
				}
				?>
                </tr>
                <?php
                $desabilita = possui_perfil(PFLCOD_CALL_CENTER);
                $btnExc     = '<img src="/imagens/excluir_01.gif" style="cursor:pointer;" title="Não é possível excluir essa supervisão" border="0" onclick=" alert(\'Não é possível excluir essa supervisão\')"> ';
                $btnExcHab  = '<img src="/imagens/excluir.gif" border="0" title="Excluir" style="cursor:pointer;" onclick="javascript:excluirSupervisão( %s );">';
                $btnEdt     = '<img src="/imagens/alterar_pb.gif" style="cursor:pointer;" title="Não é possível editar essa supervisão" border="0" onclick=" alert(\'Não é possível editar essa supervisão\')"> ';
                $btnEdtHab  = '<img src="/imagens/alterar.gif" border="0" title="Editar" style="cursor:pointer;" onclick="javascript:editarSupervisão( %s );">';
                $btnPrtHab  = '<img src="/imagens/print.png" 	border="0"	title="Editar" 	style="cursor:pointer;" onclick="javascript:imprimirLaudo( %s );"> ';
                    
                $totReg     = count( $dados );
		$i	    = 1;
                foreach($dados as $chave){ 
                ?>
                <tr <? if( $i%2 ) print("bgcolor=#f0f0f0") ?>>
                    <td align="center">
                    <?php
					if ( ($totReg != $i) || ($desabilita == 1) ){
						echo $btnEdt . "&nbsp;";	
						echo $btnExc;	
					}else{
						echo sprintf($btnEdtHab . "&nbsp;", $chave['sueid']);
						echo sprintf($btnExcHab, $chave['sueid']);
					}
					echo '&nbsp;'.sprintf( $btnPrtHab, $chave['sueid'] );
                    ?>
                    </td>
                    <td>
                        <? print($chave["sosnum"]); ?>
                    </td>
                    <td>
                        <? print($chave["esddsc"]); ?>
                    </td>
                    <td align="center">
                        <? print($chave["suedtsupervisao"]); ?>
                    </td>
                    <td align="center">
                        <? print($chave["suedtcadastro"]); ?>
                    </td>
                    <td>
                    	<img border="0" onclick='envia_email("<?php echo $chave["entcpf"] ?>");' title="Enviar e-mail ao Responsável" src="../imagens/email.gif" style="cursor: pointer;"/>
                        <? print($chave["entnome"]); ?>
                    </td>
                    <td>
                        <? print($chave["suecargovistoriador"]); ?>
                    </td>
                    <td>
                    	<img border="0" onclick='envia_email("<?php echo $chave["usucpf"] ?>");' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
                        <? print($chave["usunome"]); ?>
                    </td>
                    <td align="center">
                        <? print($chave["suefuncionamento"]); ?>
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
			$supervisaoOS = new Supervisao_Os();
            if ( $supervisaoOS->possuiNovaOSPorEmpid( $empid, array('esdid' => ESDID_OS_EXECUCAO) ) ){
            ?>
            	<input style="cursor:pointer; margin-left:15px;" type="button" name="inserir_supervisão" value="Inserir Nova Supervisão" onclick="window.location='?modulo=principal/cadVistoriaEmpresa&acao=A'" />
			<?php
            }else{
            	echo '&nbsp;&nbsp;&nbsp;<font style="color: red;">Não há nenhuma <b>OS</b> nova que vincule esta obra.</font>';
            }
            ?>
            </td>
        </tr>        
</table>
<script type="text/javascript">

function imprimirLaudo( sueid ){
	return windowOpen( '?modulo=principal/popupImpressaoLaudo&acao=A&sueid=' + sueid,'blank',
						'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}
            
function excluirSupervisão( sueid ){
    if(confirm("Deseja realmente excluir essa supervisão?")){
    	$('#sueid').val( sueid );
    	$('#operacao').val( 'apagar' );
    	$('#formulario').submit();
    }
}

function editarSupervisão( sueid ){
    	$('#sueid').val( sueid );
    	$('#operacao').val( 'editar' );
    	$('#formulario').submit();
}
</script>
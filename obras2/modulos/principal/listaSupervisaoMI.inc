<?php
// empreendimento || obra || orgao
verificaSessao( 'obra' );

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

switch ( $_POST['op'] ){
    case 'alterar':
        $_SESSION['obras2']['smiid'] = $_POST['smiid'];
        header('Location: ?modulo=principal/cadSupervisaoMI&acao=E');
        die;
    case 'apagar':
        $supervisaoMi = new SupervisaoMi( $_POST['smiid'] );
        $supervisaoMi->smistatus = 'I';
        $supervisaoMi->salvar();
        
        $db->commit();
        die("<script>
                alert('Operação realizada com sucesso!');
                location.href = '?modulo=principal/listaSupervisaoMI&acao=A';
         </script>");
}

//Chamada de programa
include  APPRAIZ.'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);

echo cabecalhoObra($obrid);
monta_titulo('Lista de Medição MI', '');
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<div style="height: 300px; overflow: auto;"> 
<?php
$supervisaoMi   = new SupervisaoMi();
$param          = array();
$param['empid'] = $empid;
$sql            = $supervisaoMi->listaSql($param);
$cabecalho      = array('Ação',
                    'Data da Supervisão',
                    'Data de Inclusão',
                    'Responsável',
                    'Cargo do Responsável',
                    'Inserido Por',
		    'Unidade em Funcionamento?');
$db->monta_lista($sql, $cabecalho, 20, 5, 'N', 'center', $par2, 'formulario');
?>
</div>
<form id="formSupervisaoMi" action="" method="post">
    <center>
	<input type="hidden" name="op" id="op">
	<input type="hidden" name="smiid" id="smiid">
	<div style="width: 95%; height: 23px; background: #C0C0C0;">
            <input type="button" value="Nova Medição" onclick="novaSupervisaoMI();" style="float: left; margin-left: 30px;">
            <input type="button" value="Voltar para obra" onclick="irObra(<?=$obrid?>);" style="float: left; margin-left: 30px;">
        </div>
    </center>
</form>
<script type="text/javascript">
<!--
function novaSupervisaoMI(){
    location.href = '?modulo=principal/cadSupervisaoMI&acao=A';
}

function irObra(obrid){
    location.href = '?modulo=principal/cadObra&acao=A&obrid='+obrid;
}

function alterarSupMI( smiid ){
    $('#op').val('alterar');
    $('#smiid').val(smiid);
    $('#formSupervisaoMi').submit();
}

function excluirSupMI( smiid ){
    if ( confirm('Deseja apagar esta supervisão?') ){
        $('#op').val('apagar');
        $('#smiid').val(smiid);
        $('#formSupervisaoMi').submit();
    }
}

function abreTelaImpressao(smiid) {
    return windowOpen(
        '?modulo=principal/impressaoSupervisaoMI&acao=A&smiid='+smiid,'blank'
      , 'height=700,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes'
    );
}
//-->
</script>
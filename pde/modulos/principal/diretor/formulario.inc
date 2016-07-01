<?php
global $db;

include_once( APPRAIZ . "pde/classes/Enemdiretor.class.inc" );
include_once( APPRAIZ . "pde/classes/Enemservidor.class.inc" );

$modelDiretor = new EnemDiretor($_REQUEST['ediid']);

if ($_POST['usudescricao']) {

    $modelDiretor->popularDadosObjeto();
    $modelDiretor->usucpf = current($_POST['usucpf']);
    $modelDiretor->edistatus = 'A';

    if(!$modelDiretor->usucpf){
        die("<script>
            alert('O campo [Diretor] é obrigatório!');
            window.location = 'enem.php?modulo=principal/diretor/formulario&acao=A';
		 </script>");
    }


    if(!$modelDiretor->ediid){
        $modelDiretor->edidatacadastro = date('Y-m-d H:i:s');
    }
    $modelDiretor->salvar();
    $modelDiretor->Commit();

    $sql = "delete from pde.enemservidor where ediid = '{$modelDiretor->ediid}'";
    $modelDiretor->carregar($sql);

    if(is_array($_POST['usucpfservidores'])){
        foreach($_POST['usucpfservidores'] as $usucpfservidor){
            $modelServidor = new EnemServidor();
            $modelServidor->ediid = $modelDiretor->ediid;
            $modelServidor->usucpf = $usucpfservidor;
            $modelServidor->eseistatus = 'A';
            $modelServidor->esedatacadastro = date('Y-m-d H:i:s');
            $modelServidor->salvar();
            $modelServidor->Commit();
            unset($modelServidor);
        }
    }
	
    if(is_array($_POST['usucnpjservidores'])){
        foreach($_POST['usucnpjservidores'] as $usucnpjservidor){
            $modelServidor = new EnemServidor();
            $modelServidor->ediid = $modelDiretor->ediid;
            $modelServidor->entid = $usucnpjservidor;
            $modelServidor->eseistatus = 'A';
            $modelServidor->esedatacadastro = date('Y-m-d H:i:s');
            $modelServidor->salvar();
            $modelServidor->Commit();
            unset($modelServidor);
        }
    }

    die("<script>
            alert('Operação realizada com sucesso!');
            window.location = 'enem.php?modulo=principal/diretor/formulario&acao=A&ediid=" . $modelDiretor->ediid . "';
		 </script>");
}
?>

<link href="css/jquery-ui/custom-theme/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="js/jquery-1.9.1.js"></script>
<script src="js/jquery-ui-1.10.3.custom.js"></script>
<link href="css/estilo.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/funcoes.js"></script>
<?php
extract($_REQUEST);

include APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba($abacod_tela, $url, '');
monta_titulo($titulo_modulo, '&nbsp;');
?>

<form action="" method="post" name="formulario" id="formulario">
    <input type="hidden" name="ediid" id="ediid" value="<?php echo $modelDiretor->ediid; ?>"/>
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Diretor:</td>
            <td>
                <?php
				//PESSOAS FÍSICAS
                $sql = "SELECT DISTINCT
							u.usucpf AS codigo,
							u.usunome AS descricao
						FROM seguranca.usuario u
						INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
						WHERE us.sisid = 24
						ORDER BY u.usunome";
				
				//PESSOAS JURÍDICAS
				$sqlJuridica = "SELECT DISTINCT
									ent.entid as codigo, 
									ent.entnome as descricao 
								FROM 
									entidade.entidade ent 
								INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
								WHERE 
									fen.funid IN (90, 91, 92)
								ORDER BY descricao";
						
                $sqlCarregar = "select usucpf as codigo, usunome as descricao from seguranca.usuario where usucpf = '{$modelDiretor->usucpf}'";
                $usucpf = $modelDiretor->carregar($sqlCarregar);
                combo_popup( "usucpf", $sql, "Diretor", "400x400", 1, array(), "", "S", false, false, 1, 400 );
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Descrição:</td>
            <td><?php echo campo_texto('usudescricao', 'S', 'S', 'Descrição', 40, 250, '', '', '', '', '', 'id="usudescricao"', '', $modelDiretor->usudescricao); ?></td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Servidores:</td>
            <td>
                <?php
                if($modelDiretor->ediid){
                    $sqlCarregar = "select usucpf as codigo, usunome as descricao from seguranca.usuario where usucpf in (select s.usucpf from pde.enemservidor s where ediid = '{$modelDiretor->ediid}') ";
                    $usucpfservidores = $modelDiretor->carregar($sqlCarregar);
                }
                combo_popup( "usucpfservidores", $sql, "Servidores", "400x400", 50, array(), "", "S", false, false, 5, 400 );
                ?>
            </td>
        </tr>
		<tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Empresas:</td>
            <td>
                <?php
                if($modelDiretor->ediid){
                    $sqlCarregar = "SELECT ent.entid as codigo, ent.entnome as descricao FROM entidade.entidade ent WHERE ent.entid in (select entid from pde.enemservidor s where ediid = '{$modelDiretor->ediid}') ";
                    $usucnpjservidores = $modelDiretor->carregar($sqlCarregar);
                }
                combo_popup( "usucnpjservidores", $sqlJuridica, "Empresas", "400x400", 50, array(), "", "S", false, false, 5, 400 );
                ?>
            </td>
        </tr>
        <tr id="tr_botoes_acao" style="background-color: #cccccc">
            <td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
            <td>
                <input type="button" name="botao_enviar" id="botao_enviar" value="Salvar" />
                <input type="button" name="botao_voltar" id="botao_voltar" value="Voltar" />
            </td>
        </tr>
    </table>
</form>

<script type="text/javascript">
    jQuery(function() {

        jQuery('#botao_voltar').click(function() {
            window.location = 'enem.php?modulo=principal/diretor/listar&acao=A';
        });

        jQuery('#botao_enviar').click(function() {

            selectAllOptions(document.getElementById('usucpf'));
            selectAllOptions(document.getElementById('usucpfservidores'));
            selectAllOptions(document.getElementById('usucnpjservidores'));

            if (!jQuery('#usucpf').val()) {
                alert('O campo [Diretor] é obrigatório.');
                return false;
            }

            if (!jQuery('#usucpfservidores').val()) {
                alert('Selecione ao menos um diretor');
                return false;
            }

            if (!jQuery('#usudescricao').val()) {
                alert('O campo [Descrição] é obrigatório.');
                return false;
            }

            jQuery('#formulario').submit();
        });


    });
</script>
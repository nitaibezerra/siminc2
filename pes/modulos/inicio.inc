<?
//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";

// Se tiver o cpf na sessao.
if (isset( $_SESSION['usucpf'] )) {

    // Busca orgao, uo e entidade que o usuario teve ultimo acesso para por na barra onde lista.
    $model = new Model_Entidade();
    $entidadeUltimoAcesso = $model->carregarTudoDaUltimaEntidadePorUsuario( $_SESSION['usucpf'] );

    // Se nao tiver ultima entidade, seleciona a primeira encontrada para ficar como ultimo acesso.
    if (!$entidadeUltimoAcesso) {

        $entidades = $model->carregarTudoDaEntidadePorUsuario( $_SESSION['usucpf'] );
        if ($entidades) {
            $entidadeUltimoAcesso = $entidades[0];

            // Colocando a primeira entidade que foi encontrada como ultimo acesso.
            $controllerUsuario = new Controller_Usuario();
            $_POST['entcodigo'] = $entidadeUltimoAcesso['entcodigo'];
            $controllerUsuario->selecionarEntidadeAction();
        }
    }

    // Se tiver entidade, exibe a barra com os dados dela se nao ele exibe uma mensagem de aviso.
    if (!empty($entidadeUltimoAcesso)) {
        
        // Colocando dados da entidade na sessao.
        $_SESSION['pes']['nome_orgao'] = $entidadeUltimoAcesso['orgnome'];
        $_SESSION['pes']['codigo_orgao'] = $entidadeUltimoAcesso['orgcodigo'];
        $_SESSION['pes']['nome_unidadeorcamentaria'] = $entidadeUltimoAcesso['uornome'];
        $_SESSION['pes']['codigo_unidadeorcamentaria'] = $entidadeUltimoAcesso['uorcodigo'];
        $_SESSION['pes']['nome_entidade'] = $entidadeUltimoAcesso['entnome'];
        $_SESSION['pes']['codigo_entidade'] = $entidadeUltimoAcesso['entcodigo'];

        // Exibindo barra com os dados recursivos da entidade.
        $controllerEntidade = new Controller_Usuario();
        echo $controllerEntidade->barraEntidadeUsuarioAction();
    } else {
        // Exibindo barra com os dados recursivos da entidade.
        $controllerEntidade = new Controller_Usuario();
        echo $controllerEntidade->barraEntidadeUsuarioAction();

        echo '<script lang="javascript">
                    msg(null, "Você não possui nenhuma entidade vinculada.<br />Favor entrar em contato com o gestor do sistema, de 09:00 às 12:00 ou de 14:00 às 18:00, por meio dos telefones: <br /> &nbsp;&nbsp;&nbsp;&nbsp; (61) 2022-8906 <br /> &nbsp;&nbsp;&nbsp;&nbsp; (61) 2022-8905 <br /> &nbsp;&nbsp;&nbsp;&nbsp; (61) 2022-8895" , null , null, 220, 500);
              </script>';
    }
}
?>
<br>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
    <tr bgcolor="#e7e7e7">
        <td><h1 style="text-align: center;">PES - Projeto Esplanada Sustentável</h1></td>
    </tr>
    <tr bgcolor="#e7e7e7">
        <td>
            <p>O PES tem por objetivo principal incentivar órgãos e instituições públicas federais (Entidade) a adotarem modelo de gestão organizacional e de processos estruturado na implementação de ações voltadas ao uso racional de recursos naturais, promovendo a sustentabilidade ambiental e socioeconômica na Administração Pública Federal.</p>
            <p>Os outros objetivos do PES são:</p>
            <ul>
                <li>Promover a sustentabilidade ambiental, econômica e social na Administração Pública Federal;</li>
                <li>Melhorar a qualidade do gasto público pela eliminação do desperdício e pela melhoria contínua da gestão dos processos;</li>
                <li>Incentivar a implementação de ações de eficiência energética nas edificações públicas;</li>
                <li>Estimular ações para o consumo racional dos recursos naturais e bens públicos;</li>
                <li>Garantir a gestão integrada de resíduos pós-consumo, inclusive a destinação ambientalmente correta;</li>
                <li>Melhorar a qualidade de vida no ambiente do trabalho; e</li>
                <li>Reconhecer e premiar as melhores práticas de eficiência na utilização dos recursos públicos, nas dimensões de economicidade e socioambientais.</li>
            </ul>

            <?php
            $sql = "select arqid from public.manual where sisid = 160 order by mandata desc, manhora desc";
            $arqid = $db->pegaUm($sql);
            ?>

            <p style="text-align: center; font-size: 12px;">
                O manual de utilização do sistema pode ser visualizado clicando-se no seguinte link: <a target="_blank" href="../mostra_arquivo.php?id=<?php echo $arqid; ?>">Manual</a>.
            </p>

        </td>
    </tr>
</table>
<!--<script language="javascript">
    var txt = "O sistema está temporariamente BLOQUEADO para inserção de dados. Somente a visualização está disponível.  Em caso de dúvidas, favor entrar em contato com o gestor do sistema, de 09:00 às 12:00 ou de 14:00 às 18:00, por meio dos telefones: <br /> &nbsp;&nbsp;&nbsp;&nbsp; (61) 2022-8906 <br /> &nbsp;&nbsp;&nbsp;&nbsp; (61) 2022-8905 <br /> &nbsp;&nbsp;&nbsp;&nbsp; (61) 2022-8895";
    msg(null, txt, null, null, '220', '700');
</script>-->
<link href="css/jquery-ui/custom-theme/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="js/jquery-1.9.1.js"></script>
<script src="js/jquery-ui-1.10.3.custom.js"></script>
<link href="css/estilo.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/funcoes.js"></script>
<div id="dialog"></div>
<?php //ver($_SESSION['pes']['nome_entidade'],d);
if(isset($_SESSION['pes']['nome_entidade']) && !empty($_SESSION['pes']['nome_entidade'])): ?>

<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem" style="text-align: left;">
    <tr bgcolor="#e7e7e7">
        <td>
            <a href="#" onclick="javascript:alterarEntidade();" style="text-decoration:none; color: #000;">
                <p>&nbsp;
                    <b>
                        <?php echo $_SESSION['pes']['nome_orgao'] ?> /
                        <?php echo $_SESSION['pes']['nome_unidadeorcamentaria'] ?> /
                        <?php echo $_SESSION['pes']['nome_entidade'] ?>
                    </b>
                </p>
            </a>
        </td>
    </tr>
</table>
<script lang="javascript">
    
    /**
     * Abre modal para o usuario selecionar a entidade que ele deseja.
     * 
     * @name modalAjax
     * @returns {void}      
     * 
     * @since 07/06/2013
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * 
     */
    function alterarEntidade()
    {
        var title = 'Selecionar Entidade';
        var data = { action : 'listarOrgao', controller : 'Usuario'};
        
        modalAjax( title , data );
    }
</script>
<?php elseif($_SESSION['favurl'] != 'pes.php?modulo=inicio&acao=C'): ?>
    <script lang="javascript">
            window.location.href = 'pes.php?modulo=inicio&acao=C';
    </script>
<?php exit; endif ?>

